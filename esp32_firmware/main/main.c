/**
 * @file main.c
 * @brief Ponto de entrada e orquestrador principal do firmware do ESP32
 * (Coordenador Mesh).
 * @details Este arquivo e responsavel por inicializar a infraestrutura do
 * sistema, como NVS Flash, modulo criptografico, pilha BLE Mesh, modem celular
 * SIMCom 7663E (AT commands), cliente MQTT, e a tarefa central de
 * roteamento/despacho de mensagens criptografadas para a rede local e nuvem.
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
#include <string.h>
#include <time.h>

#include "ble_mobile.h"
#include "mesh_coordinator.h"
#include "mqtt_publisher.h"
#include "secure_payload.h"
#include "simcom_driver.h"
#include "stm32_uart.h"
#include "stm32_cmd.h"
#include "offline_cache.h"
#include "ota_manager.h"
#include "wifi_driver.h"
#include "animal_db.h"
#include "cmd_parser.h"
#include "esp32_logger.h"
#include "private_configs.h"
#include "rfid_dedup.h"

static const char *TAG = "MAIN";

static simcom_gps_data_t gps_data;
static TaskHandle_t gps_reader_task_handle = NULL;
static volatile bool gps_read_pending = false;
static volatile bool gps_read_done = false;

/* Forward declaration - definidas apos app_main */
static void system_orchestrator_task(void *pvParameters);
static void gps_reader_task(void *pvParameters);

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

  ESP_LOGI(TAG, "Task despachante iniciada com sucesso.");

  while (1) {
    if (xQueueReceive(stm32_data_queue, &raw_msg, pdMS_TO_TICKS(1000)) == pdTRUE) {
      memset(json_buf, 0, sizeof(json_buf));
      memset(json_mqtt, 0, sizeof(json_mqtt));

      // 1. Reconstroi o JSON para BLE Mesh (formato original com type/model/tag)
      if (raw_msg.type == DATA_TYPE_RFID || raw_msg.type == DATA_TYPE_RFID_WITH_ACCEL) {
        // Dedup: se mesma tag lida nos ultimos 60s, pula processamento
        if (rfid_dedup_is_duplicate(raw_msg.tag, raw_msg.model,
            bastao_current_status.gps_latitude,
            bastao_current_status.gps_longitude,
            bastao_current_status.gps_fix)) {
          continue;
        }

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
        strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%dT%H:%M:%S-04:00", tm_info);

        // Monta JSON para MQTT no formato esperado pelo sistemaBastao
        // nivel_bateria = percentual 0-100 (nao tensao bruta)
        // Localizacao: GPS优先, fallback para torre celular
        double lat = 0.0;
        double lon = 0.0;
        bool has_location = false;

        if (bastao_current_status.gps_fix) {
            lat = bastao_current_status.gps_latitude;
            lon = bastao_current_status.gps_longitude;
            has_location = true;
        } else if (simcom_driver_is_cell_tower_valid()) {
            /* Tenta geolocalizacao por torre celular (Mozilla Location Service) */
            double cell_lat = 0.0, cell_lon = 0.0;
            if (simcom_driver_get_cell_tower_location(&cell_lat, &cell_lon) == ESP_OK) {
                lat = cell_lat;
                lon = cell_lon;
                has_location = true;
                ESP_LOGI(TAG, "Usando localizacao via torre celular: %.6f, %.6f", lat, lon);
            }
        }

        /* Publica MQTT apenas se tiver localizacao (GPS ou torre) */
        if (!has_location) {
            ESP_LOGW(TAG, "Tag %s descartada do MQTT (sem GPS e sem torre celular).", raw_msg.tag);
        }
        float batt_v = bastao_current_status.battery_voltage > 0.0f ? bastao_current_status.battery_voltage : 8.4f;
        int batt_pct;
        if (batt_v >= 8.80f) batt_pct = 100;
        else if (batt_v <= 7.50f) batt_pct = 0;
        else batt_pct = (int)((batt_v - 7.50f) / (8.80f - 7.50f) * 100.0f);

        snprintf(json_mqtt, sizeof(json_mqtt),
                 "{\"id_brinco\":\"%s\",\"latitude\":%.6f,\"longitude\":%.6f,\"nivel_bateria\":%d,\"timestamp_rtc\":\"%s\"}",
                 raw_msg.tag, lat, lon, batt_pct, timestamp_str);

        ble_mobile_notify_tag(json_buf);
        bastao_current_status.tags_read_count++;

        if (raw_msg.movement || raw_msg.type == DATA_TYPE_RFID) {
          stm32_cmd_send_buzzer(STM32_CMD_BUZZER_SHORT);
        }

        // 3. Enfileira JSON cru para MQTT (sistemaBastao)
        //    A criptografia AES-256 e feita na mqtt_publish_task (Core 1, prio 4)
        //    para nao bloquear o dispatcher com a sobrecarga computacional do AES.
        //    Publica APENAS se tiver localizacao (GPS ou torre celular).
        if (has_location && json_mqtt[0] != '\0') {
          ESP_LOGD(TAG, "JSON MQTT: %s", json_mqtt);
          esp_err_t mqtt_err = mqtt_publisher_enqueue_raw(bastao_network_config.mqtt.topic_telemetry,
                                           json_mqtt, 1);
          if (mqtt_err != ESP_OK) {
            ESP_LOGW(TAG, "Falha ao enfileirar MQTT. Salvando no cache...");
          }
        } else if (!has_location) {
          ESP_LOGW(TAG, "Tag %s descartada do MQTT (sem localizacao).", raw_msg.tag);
        }

      } else if (raw_msg.type == DATA_TYPE_BATTERY) {
        float pct = 0;
        if (raw_msg.battery_v >= 8.80f) pct = 100;
        else if (raw_msg.battery_v <= 7.50f) pct = 0;
        else pct = (raw_msg.battery_v - 7.50f) / (8.80f - 7.50f) * 100.0f;

        snprintf(json_buf, sizeof(json_buf),
                 "{\"type\":\"batt\",\"volt\":%.2f,\"pct\":%d}",
                 raw_msg.battery_v, (int)pct);

        bastao_current_status.battery_voltage = raw_msg.battery_v;

        if (raw_msg.battery_v < 7.7f) {
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

      // 2. Envia JSON simples (sem criptografia) via BLE Mesh para K10
      //    A seguranca e garantida pela criptografia de link layer do BLE Mesh
      //    (AppKey + NetKey). O K10 nao tem capacidade de descriptografia AES-256.
      esp_err_t err = mesh_coordinator_send_data(json_buf);
      if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao encaminhar dados via BLE Mesh.");
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
            if (!simcom_driver_is_suspended()) {
                ESP_LOGI(TAG, "[Orquestrador] Modo WIFI_ONLY: Suspendendo celular.");
                simcom_driver_set_suspended(true);
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
            if (simcom_driver_is_suspended()) {
                ESP_LOGI(TAG, "[Orquestrador] Modo CELLULAR_ONLY: Reativando celular.");
                simcom_driver_set_suspended(false);
            }
            // NAO desconecta o Wi-Fi — ele permanece ativo para debug remoto
            // via Telnet (porta 23). O MQTT nao trafega por Wi-Fi pois o
            // cliente esp_mqtt nao foi inicializado neste modo (mqtt_connected=false).
            wifi_reconnect_timer = 0;
            break;
        }

        case NETWORK_MODE_WIFI_CELLULAR:
        case NETWORK_MODE_AUTO: {
            if (wifi_connected) {
                if (!simcom_driver_is_suspended()) {
                    ESP_LOGI(TAG, "[Orquestrador] Wi-Fi conectado. Suspendendo celular para economizar dados/energia.");
                    simcom_driver_set_suspended(true);
                }
                wifi_reconnect_timer = 0;
            } else {
                if (simcom_driver_is_suspended()) {
                    ESP_LOGI(TAG, "[Orquestrador] Wi-Fi desconectado. Reativando celular...");
                    simcom_driver_set_suspended(false);
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

  // Configura nivel de debug para rede e conexao celular
  esp_log_level_set("SIMCOM_DRV", ESP_LOG_DEBUG);

  // ----------------------------------------------------------------
  // PASSO 0: Inicializa a pilha TCP/IP e o Event Loop do sistema.
  // DEVE ser chamado ANTES de qualquer modulo de rede (Wi-Fi, celular).
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

  if (offline_cache_init() == ESP_OK) {
    offline_cache_sync_task_start(3);
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar o Cache Offline SPIFFS.");
  }

  ota_manager_init();

  ble_mobile_load_network_config();

  // Inicializa Wi-Fi SEMPRE (independente do modo de rede) para permitir
  // debug remoto via Telnet na porta 23. O MQTT sobre Wi-Fi so sera usado
  // se o modo de rede nao for CELLULAR_ONLY (controlado pelo manage_connectivity).
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
    ESP_LOGI(TAG, "Logger Telnet ativo na porta 23 -- conecte-se ao AP Bastao-XXXXXX, telnet %s", wifi_driver_get_ap_ip());
    ble_mobile_log_enable(true);
  }

  // 2. Instancia a fila global compartilhada para trafego de dados recebidos do
  // STM32
  stm32_data_queue = xQueueCreate(50, sizeof(stm32_data_t));
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
    if (stm32_uart_rx_task_start(6) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar a task stm32_uart_rx.");
    }

    // 6b. Aguarda boot do STM32 e aplica reset para sincronizar
    vTaskDelay(pdMS_TO_TICKS(500));
    stm32_uart_reset_stm32();
    vTaskDelay(pdMS_TO_TICKS(300));

  }

  // 6c. Inicializa o modulo de deduplicacao RFID
  rfid_dedup_init();

  // 7. Dispara a tarefa despachante com prioridade 6 (superior a task serial
  // para escoamento rapido)
  if (xTaskCreatePinnedToCore(dispatcher_task, "dispatcher_task", 6144, NULL, 6, NULL, 0) !=
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

  // Substitui placeholder 000000000000 pelo MAC real nos topicos MQTT
  char mac_str[13];
  snprintf(mac_str, sizeof(mac_str), "%02X%02X%02X%02X%02X%02X",
           _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
  char *p = strstr(active_mqtt_config.topic_telemetry, "000000000000");
  if (p) memcpy(p, mac_str, 12);
  p = strstr(active_mqtt_config.topic_gps, "000000000000");
  if (p) memcpy(p, mac_str, 12);
  p = strstr(bastao_network_config.mqtt.topic_telemetry, "000000000000");
  if (p) memcpy(p, mac_str, 12);
  p = strstr(bastao_network_config.mqtt.topic_gps, "000000000000");
  if (p) memcpy(p, mac_str, 12);
  ESP_LOGI(TAG, "Topicos MQTT ajustados para MAC %s", mac_str);

  // Inicializa o publicador MQTT sempre (fila/task rodando)
  if (mqtt_publisher_init(&active_mqtt_config) == ESP_OK) {
    if (mqtt_publisher_task_start(4) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar task de publicacao MQTT.");
    }
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar publicador MQTT.");
  }

  // 8. Inicializa o modem celular SIMCom 7663E
  ESP_LOGI(TAG, "Inicializando modulo celular SIMCom 7663E...");

  // Carrega dados SIM salvos de boot anterior (para exibir enquanto o probe roda)
  ble_mobile_load_sim_data();

  if (simcom_driver_init() == ESP_OK) {
    simcom_apn_config_t active_apn = {0};
    strncpy(active_apn.apn, bastao_network_config.cellular.apn, sizeof(active_apn.apn) - 1);
    strncpy(active_apn.user, bastao_network_config.cellular.user, sizeof(active_apn.user) - 1);
    strncpy(active_apn.password, bastao_network_config.cellular.password, sizeof(active_apn.password) - 1);

    // Persiste dados SIM na NVS (IMEI, MSISDN, ICCID, operadora)
    ble_mobile_save_sim_data();

    if (simcom_driver_configure_apn(&active_apn) == ESP_OK) {
      // Atualiza dados SIM com operadora e sinal recem-coletados
      ble_mobile_save_sim_data();

      // Acelera fix GPS: baixa dados de efeméride do servidor AGNSS via 4G
      // (AT+CAGPS). Com dados AGNSS, fix ocorre em 2-5s em vez de 30s+.
      ESP_LOGI(TAG, "[GPS] Configurando A-GPS para fix acelerado...");
      if (simcom_driver_download_agps() == ESP_OK) {
        ESP_LOGI(TAG, "[GPS] A-GPS configurado com sucesso.");
      } else {
        ESP_LOGW(TAG, "[GPS] A-GPS falhou. GPS usara cold start puro (30s+).");
      }

      if (!wifi_driver_is_connected()) {
        if (simcom_driver_mqtt_connect(&active_mqtt_config) == ESP_OK) {
          ESP_LOGI(TAG, "Conectividade celular MQTT ativa.");
        } else {
          ESP_LOGE(TAG, "Falha inicial ao estabelecer sessao MQTT (sera tentado pelo Watchdog).");
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
  simcom_driver_watchdog_start(3);

  // 11. Dispara a tarefa de orquestracao do sistema com stack de 8KB
  if (xTaskCreatePinnedToCore(system_orchestrator_task, "sys_orchestr", 8192, NULL, 5, NULL, 0) != pdPASS) {
    ESP_LOGE(TAG, "Falha ao criar a tarefa de orquestracao do sistema!");
  }

  // 12. Dispara a tarefa de leitura GPS (background, nao bloqueia o orchestrator)
  if (xTaskCreatePinnedToCore(gps_reader_task, "gps_reader", 4096, NULL, 3, &gps_reader_task_handle, 1) != pdPASS) {
    ESP_LOGE(TAG, "Falha ao criar a tarefa de leitura GPS!");
  }

  // Aguarda tasks iniciarem e loga uso de stack de cada uma
  vTaskDelay(pdMS_TO_TICKS(200));
  {
    static const char * const task_names[] = {
      "dispatcher_task", "stm32_uart_rx", "simcom_rx",
      "sys_orchestr", "mqtt_pub_task", "simcom_wd",
      "log_processor", "telnet_logger", "cache_sync_task",
      "gps_reader"
    };
    ESP_LOGI(TAG, "=== Monitoramento de Stack FreeRTOS ===");
    for (int i = 0; i < sizeof(task_names) / sizeof(task_names[0]); i++) {
      TaskHandle_t h = xTaskGetHandle(task_names[i]);
      if (h != NULL) {
        UBaseType_t free = uxTaskGetStackHighWaterMark(h);
        ESP_LOGI(TAG, "  %-22s  %4u bytes livres", task_names[i], free);
      } else {
        ESP_LOGW(TAG, "  %-22s  (handle nao encontrado)", task_names[i]);
      }
    }
    ESP_LOGI(TAG, "=========================================");
  }

  ESP_LOGI(TAG, "Inicializacao de app_main concluida. Deletando task main.");
}

/**
 * @brief Task de orquestracao do sistema (connectivity, GPS, status, heartbeat).
 */
static void system_orchestrator_task(void *pvParameters) {
  // Configura timezone GMT-4 para America/Manaus
  setenv("TZ", "AMT+4", 1);
  tzset();
  ESP_LOGI(TAG, "Timezone configurado: GMT-4 (AMT+4)");
  ESP_LOGI(TAG, "Tarefa de orquestracao do sistema iniciada.");
  uint32_t gps_tick = 0;
  uint8_t stm32_dead_count = 0;

  while (1) {
    manage_connectivity();

    bastao_current_status.cellular_connected = simcom_driver_is_connected();
    bastao_current_status.mqtt_connected = mqtt_publisher_is_connected();
    ble_mobile_update_status(&bastao_current_status);

    if (!stm32_uart_is_stm32_alive()) {
        bastao_current_status.stm32_alive = 0;
        if (stm32_uart_has_ever_been_alive()) {
            stm32_dead_count++;
            if (stm32_dead_count >= 120) {
                ESP_LOGE(TAG, "STM32 sem heartbeat por >120s. Aplicando reset via GPIO...");
                stm32_uart_reset_stm32();
                stm32_dead_count = 0;
            }
        }
    } else {
        bastao_current_status.stm32_alive = 1;
        stm32_dead_count = 0;
    }

    // Sincronizacao de horario via torre celular (AT+CCLK) — NON-BLOCKING
    // Se o mutex do modem estiver ocupado (ex: MQTT publicando), pula este ciclo.
    // O time sync roda a cada 30s ate o clock ser acertado, entao nao ha pressa.
    {
        time_t now = time(NULL);
        if (now < 1700000000 && simcom_driver_get_state() >= SIMCOM_STATE_REGISTERED) {
            static uint32_t time_sync_tick = 0;
            time_sync_tick++;
            if (time_sync_tick >= 30) {
                time_sync_tick = 0;
                if (!simcom_driver_is_busy()) {
                    ESP_LOGI(TAG, "Hora nao sincronizada. Tentando via torre celular (AT+CCLK)...");
                    simcom_driver_sync_time_from_tower();
                } else {
                    ESP_LOGD(TAG, "Time sync: modem ocupado. Tentando no proximo ciclo.");
                }
            }
        }
    }

    // Leitura periodica do GPS com intervalo adaptativo — NON-BLOCKING
    // A leitura GPS acontece em task separada (gps_reader_task, prio 3, Core 1)
    // para nao travar o loop de orquestracao.
    // - Sem fix: poll a cada 2s (busca rapida de sinal)
    // - Com fix: poll a cada 30s (monitoramento normal)
    {
      uint32_t gps_interval = bastao_current_status.gps_fix ? 30 : 2;
      gps_tick++;
      if (gps_tick >= gps_interval) {
        gps_tick = 0;
        // Dispara leitura GPS na task especializada (nao bloqueia aqui)
        if (!gps_read_pending && gps_reader_task_handle != NULL) {
          gps_read_pending = true;
          gps_read_done = false;
          xTaskNotify(gps_reader_task_handle, 1, eSetValueWithOverwrite);
        }
      }

      // Verifica se a leitura GPS foi concluida pela task separada
      if (gps_read_done) {
        gps_read_done = false;
        gps_read_pending = false;

        // Envia dados GPS para K10 via Mesh
        {
          char mesh_json[256];
          snprintf(mesh_json, sizeof(mesh_json),
                   "{\"type\":\"gps\",\"lat\":%.6f,\"lon\":%.6f,\"fix\":%d,\"alt\":%.1f,\"speed\":%.1f}",
                   bastao_current_status.gps_latitude,
                   bastao_current_status.gps_longitude,
                   bastao_current_status.gps_fix ? 1 : 0,
                   gps_data.valid ? gps_data.altitude : 0.0f,
                   gps_data.valid ? gps_data.speed_kmh : 0.0f);
          mesh_coordinator_send_data(mesh_json);
        }

        // Envia status do gateway para K10 via Mesh
        {
          char mesh_json[128];
          snprintf(mesh_json, sizeof(mesh_json),
                   "{\"type\":\"status\",\"rfid_conn\":%d,\"wifi_active\":%d}",
                   bastao_current_status.stm32_alive ? 1 : 0,
                   wifi_driver_is_connected() ? 1 : 0);
          mesh_coordinator_send_data(mesh_json);
        }
      }
    }

    // Envia status da rede celular para K10 via Mesh (a cada 60s)
    // Usa RSSI em cache — nao envia comando AT para evitar travar o modem.
    // O watchdog (simcom_wd, prio 3, Core 1) atualiza o cache a cada 10s.
    {
      static uint32_t cell_tick = 0;
      cell_tick++;
      if (cell_tick >= 60) {
        cell_tick = 0;
        char mesh_json[192];
        int rssi = simcom_driver_get_cached_rssi();
        snprintf(mesh_json, sizeof(mesh_json),
                 "{\"type\":\"cell\",\"rssi\":%d,\"connected\":%d,\"operator\":\"%s\"}",
                 rssi,
                 bastao_current_status.cellular_connected ? 1 : 0,
                 bastao_current_status.sim_operator[0]);
        mesh_coordinator_send_data(mesh_json);
      }
    }

    // Atualiza cache de localizacao por torre celular (a cada 5 min)
    // HTTP POST para Mozilla Location Service — NAO bloqueia o dispatcher.
    // O resultado e usado pelo dispatcher quando GPS nao tem fix.
    {
      static uint32_t cell_loc_tick = 0;
      cell_loc_tick++;
      if (cell_loc_tick >= 300) {
        cell_loc_tick = 0;
        if (!bastao_current_status.gps_fix) {
          // cell_tower_get_location() usa AT+HTTPINIT do modem (nao precisa Wi-Fi)
          // Ela bloqueia o mutex internamente via at_send_cmd, com timeout longo
          ESP_LOGD(TAG, "[CELL] Atualizando cache de localizacao por torre...");
          simcom_driver_update_cell_tower_cache();
        }
      }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

/**
 * @brief Task dedicada a leitura GPS via modem SIMCom.
 *
 * Opera em background (prio 3, Core 1) para nao bloquear o orchestrator.
 * Acionada via notificacao do orchestrator quando GPS precisa ser lido.
 * Atualiza gps_data e bastao_current_status diretamente.
 */
static void gps_reader_task(void *pvParameters) {
  uint32_t notification_value = 0;
  while (1) {
    // Aguarda notificacao do orchestrator (com timeout de 5s)
    if (xTaskNotifyWait(0, ULONG_MAX, &notification_value, pdMS_TO_TICKS(5000)) == pdTRUE) {
      // Chama get_gps diretamente — ela ja gerencia o mutex internamente
      // com timeout longo (15s para AT+CGPSINFO). Nao precisa lock separado.
      simcom_gps_data_t gps_new;
      if (simcom_driver_get_gps(&gps_new) == ESP_OK && gps_new.valid) {
        bastao_current_status.gps_latitude = gps_new.latitude;
        bastao_current_status.gps_longitude = gps_new.longitude;
        if (!bastao_current_status.gps_fix) {
          bastao_current_status.gps_fix = true;
          ESP_LOGI(TAG, "GPS FIX OBTIDO! Mudando para polling de 30s.");
        }
        gps_data = gps_new;
        ESP_LOGI(TAG, "GPS atualizado: %.6f, %.6f",
                 gps_data.latitude, gps_data.longitude);
      }
      gps_read_done = true;
    }
  }
}