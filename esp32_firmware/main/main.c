/**
 * @file main.c
 * @brief Ponto de entrada e orquestrador principal do firmware do ESP32
 * (Coordenador Mesh).
 * @details Este arquivo e responsavel por inicializar a infraestrutura do
 * sistema, como NVS Flash, modulo criptografico, pilha BLE Mesh, modem celular
 * SIMCom 7663E (PPP), cliente MQTT, e a tarefa central de roteamento/despacho
 * de mensagens criptografadas para a rede local e para a nuvem.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <time.h>

#include "ble_mobile.h"
#include "mesh_coordinator.h"
#include "mqtt_publisher.h"
#include "secure_payload.h"
#include "simcom_ppp.h"
#include "stm32_uart.h"
#include "stm32_cmd.h"
#include "offline_cache.h"
#include "ota_manager.h"
#include "wifi_driver.h"
#include "animal_db.h"
#include "esp_power.h"
#include "cmd_parser.h"
#include "esp32_logger.h"
#include "private_configs.h"

static const char *TAG = "MAIN";

static simcom_gps_data_t gps_data;

/**
 * @brief Chave simetrica estatica padrao para criptografia AES-256 (32 bytes).
 */
static const uint8_t default_aes_key[AES_KEY_SIZE_BYTES] = PRIVATE_AES_KEY;



/**
 * @brief Task consumidora/despachante encarregada de processar a fila
 * stm32_data_queue.
 * @details Retira dados da fila estruturada preenchida pela recepcao UART,
 * serializa-os de volta em JSON padrao, aciona a criptografia AES-256 do modulo
 * secure_payload, e direciona a string criptografada em hexadecimal para a Tela
 * K10 via BLE Mesh.
 *
 * @param[in,out] pvParameters Parametros padrao do FreeRTOS (Nao utilizado).
 */
static void dispatcher_task(void *pvParameters) {
  stm32_data_t raw_msg;
  char json_buf[384];
  char json_mqtt[384];
  char encrypted_hex[768];

  ESP_LOGI(TAG, "Task despachante iniciada com sucesso.");

  while (1) {
    esp_power_update();

    if (xQueueReceive(stm32_data_queue, &raw_msg, pdMS_TO_TICKS(1000)) == pdTRUE) {
      memset(json_buf, 0, sizeof(json_buf));
      memset(json_mqtt, 0, sizeof(json_mqtt));
      memset(encrypted_hex, 0, sizeof(encrypted_hex));

      // 1. Reconstroi o JSON para BLE Mesh (formato original com type/model/tag)
      if (raw_msg.type == DATA_TYPE_RFID || raw_msg.type == DATA_TYPE_RFID_WITH_ACCEL) {
        animal_record_t anim_rec;
        esp_err_t db_err = animal_db_lookup(raw_msg.tag, &anim_rec);
        bool has_accel = (raw_msg.type == DATA_TYPE_RFID_WITH_ACCEL);

        if (db_err == ESP_OK) {
          if (has_accel) {
            snprintf(json_buf, sizeof(json_buf),
                     "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\",\"name\":\"%s\",\"weight\":%.2f,\"lot\":\"%s\",\"x\":%.2f,\"y\":%.2f,\"z\":%.2f,\"movement\":%d}",
                     raw_msg.model, raw_msg.tag, anim_rec.name, anim_rec.weight, anim_rec.lot,
                     raw_msg.accel_x, raw_msg.accel_y, raw_msg.accel_z, raw_msg.movement);
          } else {
            snprintf(json_buf, sizeof(json_buf),
                     "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\",\"name\":\"%s\",\"weight\":%.2f,\"lot\":\"%s\"}",
                     raw_msg.model, raw_msg.tag, anim_rec.name, anim_rec.weight, anim_rec.lot);
          }
        } else {
          if (has_accel) {
            snprintf(json_buf, sizeof(json_buf),
                     "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\",\"x\":%.2f,\"y\":%.2f,\"z\":%.2f,\"movement\":%d}",
                     raw_msg.model, raw_msg.tag,
                     raw_msg.accel_x, raw_msg.accel_y, raw_msg.accel_z, raw_msg.movement);
          } else {
            snprintf(json_buf, sizeof(json_buf),
                     "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\"}",
                     raw_msg.model, raw_msg.tag);
          }
        }

        // Gera timestamp ISO 8601 para o payload MQTT
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char timestamp_str[32];
        strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%dT%H:%M:%S-03:00", tm_info);

        // Monta JSON para MQTT no formato esperado pelo sistemaBastao
        double lat = bastao_current_status.gps_fix ? bastao_current_status.gps_latitude : 0.0;
        double lon = bastao_current_status.gps_fix ? bastao_current_status.gps_longitude : 0.0;
        float batt = bastao_current_status.battery_voltage > 0.0f ? bastao_current_status.battery_voltage : 8.4f;

        snprintf(json_mqtt, sizeof(json_mqtt),
                 "{\"id_brinco\":\"%s\",\"latitude\":%.6f,\"longitude\":%.6f,\"nivel_bateria\":%.2f,\"timestamp_rtc\":\"%s\"}",
                 raw_msg.tag, lat, lon, batt, timestamp_str);

        ble_mobile_notify_tag(json_buf);
        esp_power_trigger_wake();
        bastao_current_status.tags_read_count++;

        if (raw_msg.movement || raw_msg.type == DATA_TYPE_RFID) {
          stm32_cmd_send_buzzer(STM32_CMD_BUZZER_SHORT);
        }
      } else if (raw_msg.type == DATA_TYPE_BATTERY) {
        snprintf(json_buf, sizeof(json_buf),
                 "{\"type\":\"batt\",\"volt\":%.2f}", raw_msg.battery_v);

        bastao_current_status.battery_voltage = raw_msg.battery_v;

        if (raw_msg.battery_v < 9.0f) {
          stm32_cmd_send_buzzer(STM32_CMD_BUZZER_LONG);
        }
      } else if (raw_msg.type == DATA_TYPE_ACCEL) {
        snprintf(json_buf, sizeof(json_buf),
                 "{\"type\":\"accel\",\"x\":%.2f,\"y\":%.2f,\"z\":%.2f,\"movement\":%d}",
                 raw_msg.accel_x, raw_msg.accel_y, raw_msg.accel_z, raw_msg.movement);

        bastao_current_status.movement_detected = raw_msg.movement;
      } else {
        ESP_LOGW(TAG, "Mensagem recebida com tipo invalido.");
        continue;
      }

      ESP_LOGD(TAG, "JSON Mesh: %s", json_buf);

      // 2. Criptografa payload para BLE Mesh e envia para K10
      esp_err_t err = secure_payload_encrypt(json_buf, encrypted_hex, sizeof(encrypted_hex));
      if (err == ESP_OK) {
        ESP_LOGI(TAG, "Payload Mesh criptografado: %s", encrypted_hex);
        err = mesh_coordinator_send_data(encrypted_hex);
        if (err != ESP_OK) {
          ESP_LOGE(TAG, "Falha ao encaminhar dados via BLE Mesh.");
        }
      }

      // 3. Se for leitura RFID, envia tambem para MQTT (sistemaBastao)
      if ((raw_msg.type == DATA_TYPE_RFID || raw_msg.type == DATA_TYPE_RFID_WITH_ACCEL) &&
          json_mqtt[0] != '\0') {
        ESP_LOGD(TAG, "JSON MQTT: %s", json_mqtt);
        memset(encrypted_hex, 0, sizeof(encrypted_hex));
        err = secure_payload_encrypt(json_mqtt, encrypted_hex, sizeof(encrypted_hex));
        if (err == ESP_OK) {
          ESP_LOGI(TAG, "Payload MQTT criptografado: %s", encrypted_hex);
          err = mqtt_publisher_enqueue(bastao_network_config.mqtt.topic_telemetry,
                                       encrypted_hex, 1);
          if (err != ESP_OK) {
            ESP_LOGW(TAG, "Falha ao enfileirar MQTT. Salvando no cache...");
            offline_cache_write(encrypted_hex);
          }
        } else {
          ESP_LOGE(TAG, "Falha na criptografia do payload MQTT: %d", err);
        }
      }
    }
  }
}

static void manage_connectivity(void) {
    static uint32_t wifi_reconnect_timer = 0;
    network_mode_t mode = bastao_network_config.mode;
    bool wifi_connected = wifi_driver_is_connected();

    wifi_reconnect_timer++;

    switch (mode) {
        case NETWORK_MODE_WIFI_ONLY: {
            if (!simcom_ppp_is_suspended()) {
                ESP_LOGI(TAG, "[Orquestrador] Modo WIFI_ONLY: Suspendendo celular.");
                simcom_ppp_set_suspended(true);
            }
            if (!wifi_connected) {
                if (wifi_reconnect_timer >= 15) {
                    wifi_reconnect_timer = 0;
                    ESP_LOGI(TAG, "[Orquestrador] Modo WIFI_ONLY: Wi-Fi desconectado. Tentando reconectar...");
                    if (strlen(bastao_network_config.wifi.ssid) > 0) {
                        wifi_driver_connect(bastao_network_config.wifi.ssid, bastao_network_config.wifi.password);
                    }
                }
            } else {
                wifi_reconnect_timer = 0;
            }
            break;
        }

        case NETWORK_MODE_CELLULAR_ONLY: {
            if (simcom_ppp_is_suspended()) {
                ESP_LOGI(TAG, "[Orquestrador] Modo CELLULAR_ONLY: Reativando celular.");
                simcom_ppp_set_suspended(false);
            }
            if (wifi_connected) {
                ESP_LOGI(TAG, "[Orquestrador] Modo CELLULAR_ONLY: Desconectando Wi-Fi.");
                wifi_driver_disconnect();
            }
            wifi_reconnect_timer = 0;
            break;
        }

        case NETWORK_MODE_WIFI_CELLULAR:
        case NETWORK_MODE_AUTO: {
            if (wifi_connected) {
                if (!simcom_ppp_is_suspended()) {
                    ESP_LOGI(TAG, "[Orquestrador] Wi-Fi conectado. Suspendendo celular para economizar dados/energia.");
                    simcom_ppp_set_suspended(true);
                }
                wifi_reconnect_timer = 0;
            } else {
                if (simcom_ppp_is_suspended()) {
                    ESP_LOGI(TAG, "[Orquestrador] Wi-Fi desconectado. Reativando celular...");
                    simcom_ppp_set_suspended(false);
                }
                if (wifi_reconnect_timer >= 60) {
                    wifi_reconnect_timer = 0;
                    ESP_LOGI(TAG, "[Orquestrador] Modo Auto/Dual: Tentando reconectar ao Wi-Fi em segundo plano...");
                    if (strlen(bastao_network_config.wifi.ssid) > 0) {
                        wifi_driver_connect(bastao_network_config.wifi.ssid, bastao_network_config.wifi.password);
                    }
                }
            }
            break;
        }
    }
}

void app_main(void) {
  ESP_LOGI(TAG, "Inicializando o Bastao-ESP Mesh Coordinator...");

  // Configura nivel de debug para rede e conexao PPP
  esp_log_level_set("SIMCOM_PPP", ESP_LOG_DEBUG);
  esp_log_level_set("esp-netif_lwip-ppp", ESP_LOG_DEBUG);

  // ----------------------------------------------------------------
  // PASSO 0: Inicializa a pilha TCP/IP e o Event Loop do sistema.
  // DEVE ser chamado ANTES de qualquer modulo de rede (Wi-Fi, PPP).
  // ----------------------------------------------------------------
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  // 1. Inicializa e, se necessario, apaga a NVS flash
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  animal_db_init();
  esp_power_init();

  if (offline_cache_init() == ESP_OK) {
    offline_cache_sync_task_start(3);
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar o Cache Offline SPIFFS.");
  }

  ota_manager_init();

  ble_mobile_load_network_config();

  if (wifi_driver_init() == ESP_OK) {
    if (bastao_network_config.wifi.enabled &&
        strlen(bastao_network_config.wifi.ssid) > 0) {
      wifi_driver_connect(bastao_network_config.wifi.ssid,
                        bastao_network_config.wifi.password);
    }
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar o driver Wi-Fi.");
  }

  // 1f. Inicializa o logger wireless para debug remoto
  if (esp_logger_init() == ESP_OK) {
    esp_logger_start_telnet();
    ESP_LOGI(TAG, "Logger Telnet ativo na porta 23");
    ble_mobile_log_enable(true);
  }

  // 2. Instancia a fila global compartilhada para trafego de dados recebidos do
  // STM32
  stm32_data_queue = xQueueCreate(20, sizeof(stm32_data_t));
  if (stm32_data_queue == NULL) {
    ESP_LOGE(TAG,
             "Falha critica ao criar a fila stm32_data_queue. Abortando...");
    return;
  }

  // 3. Inicializa o modulo criptografico local com a chave AES-256 padrao
  if (secure_payload_init(default_aes_key) != ESP_OK) {
    ESP_LOGE(TAG, "Falha critica ao inicializar modulo de seguranca.");
    return;
  }

  // 4. Inicializa o driver BLE Mesh do ESP32
  if (mesh_coordinator_init() != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao inicializar o coordenador BLE Mesh. BLE inativo.");
  }

  // 4b. Inicializa o GATT Server BLE para comunicacao com app movel
  if (ble_mobile_init("Bastao-001") != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao inicializar GATT Server BLE Mobile.");
  }

  // 5. Inicializa o periferico UART1 para interface com o STM32 (Pinos IO13 e
  // IO14)
  if (stm32_uart_init() != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao inicializar o driver serial do STM32.");
  } else {
    // 5b. Inicializa o modulo de comandos para o STM32
    stm32_cmd_init();

    // 6. Dispara a tarefa do FreeRTOS encarregada de ler a serial e parsear os
    // JSONs
    if (stm32_uart_rx_task_start(5) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar a task stm32_uart_rx.");
    }
  }

  // 7. Dispara a tarefa despachante com prioridade 6 (superior a task serial
  // para escoamento rapido)
  if (xTaskCreate(dispatcher_task, "dispatcher_task", 6144, NULL, 6, NULL) !=
      pdPASS) {
    ESP_LOGE(TAG, "Falha ao iniciar a task dispatcher_task.");
  }

  // 8b. Inicializa o publicador MQTT (independente da rede ativa)
  mqtt_publisher_config_t active_mqtt_config = {0};
  strncpy(active_mqtt_config.broker_uri, bastao_network_config.mqtt.broker_uri, sizeof(active_mqtt_config.broker_uri) - 1);
  strncpy(active_mqtt_config.client_id, bastao_network_config.mqtt.client_id, sizeof(active_mqtt_config.client_id) - 1);
  strncpy(active_mqtt_config.topic_telemetry, bastao_network_config.mqtt.topic_telemetry, sizeof(active_mqtt_config.topic_telemetry) - 1);
  strncpy(active_mqtt_config.topic_gps, bastao_network_config.mqtt.topic_gps, sizeof(active_mqtt_config.topic_gps) - 1);

  // Usa MAC como username MQTT (numero_serie) e chave AES hex como password
  uint8_t _mac[6];
  esp_read_mac(_mac, ESP_MAC_WIFI_STA);
  snprintf(active_mqtt_config.username, sizeof(active_mqtt_config.username),
           "%02X%02X%02X%02X%02X%02X", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
  for (int _i = 0; _i < AES_KEY_SIZE_BYTES; _i++) {
    sprintf(active_mqtt_config.password + (_i * 2), "%02X", default_aes_key[_i]);
  }
  ESP_LOGI(TAG, "MQTT username (serial): %s", active_mqtt_config.username);

  if (mqtt_publisher_init(&active_mqtt_config) == ESP_OK) {
    if (mqtt_publisher_task_start(4) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar task de publicacao MQTT.");
    }
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar cliente MQTT.");
  }

  // 8. Inicializa o modem celular SIMCom 7663E e a interface PPP
  ESP_LOGI(TAG, "Inicializando modulo celular SIMCom 7663E...");
  if (simcom_ppp_init() == ESP_OK) {
    simcom_apn_config_t active_apn = {0};
    strncpy(active_apn.apn, bastao_network_config.cellular.apn, sizeof(active_apn.apn) - 1);
    strncpy(active_apn.user, bastao_network_config.cellular.user, sizeof(active_apn.user) - 1);
    strncpy(active_apn.password, bastao_network_config.cellular.password, sizeof(active_apn.password) - 1);

    if (simcom_ppp_configure_apn(&active_apn) == ESP_OK) {
      if (!wifi_driver_is_connected()) {
        if (simcom_ppp_connect() == ESP_OK) {
          ESP_LOGI(TAG, "Conectividade celular PPP ativa.");
        } else {
          ESP_LOGE(TAG, "Falha inicial ao estabelecer sessao PPP (sera tentado pelo Watchdog).");
        }
      } else {
        ESP_LOGI(TAG, "Wi-Fi ativo. Pulando conexao celular inicial.");
      }
    } else {
      ESP_LOGE(TAG, "Falha na configuracao da APN.");
    }
  } else {
    ESP_LOGE(TAG, "Falha na inicializacao do modem SIMCom.");
  }

  // 10. Inicia o watchdog de reconexao automatica do modem (prioridade 3)
  simcom_ppp_watchdog_start(3);

  // Loop principal da task de orquestracao do sistema
  // Atualiza periodicamente o status do dispositivo para leituras BLE
  uint32_t gps_tick = 0;
  while (1) {
    manage_connectivity();

    bastao_current_status.ppp_connected = simcom_ppp_is_connected();
    bastao_current_status.mqtt_connected = mqtt_publisher_is_connected();
    ble_mobile_update_status(&bastao_current_status);

    if (!stm32_uart_is_stm32_alive()) {
        bastao_current_status.stm32_alive = 0;
        ESP_LOGW(TAG, "STM32 sem resposta. Verificar conexao UART.");
    } else {
        bastao_current_status.stm32_alive = 1;
    }

    // Leitura periodica do GPS (a cada 30s, quando o modem esta em modo AT)
    // TODO: Para uso com PPP ativo, e necessario suspender PPP temporariamente
    gps_tick++;
    if (gps_tick >= 30) {
      gps_tick = 0;
      if (!simcom_ppp_is_connected() && !simcom_ppp_is_suspended()) {
        if (simcom_ppp_get_gps(&gps_data) == ESP_OK && gps_data.valid) {
          bastao_current_status.gps_latitude = gps_data.latitude;
          bastao_current_status.gps_longitude = gps_data.longitude;
          bastao_current_status.gps_fix = true;
          ESP_LOGI(TAG, "GPS atualizado: %.6f, %.6f",
                   gps_data.latitude, gps_data.longitude);
        }
      }
    }

    esp_power_update();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
