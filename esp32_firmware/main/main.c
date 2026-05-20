/**
 * @file main.c
 * @brief Ponto de entrada e orquestrador principal do firmware do ESP32
 * (Coordenador Mesh).
 * @details Este arquivo é responsável por inicializar a infraestrutura do
 * sistema, como NVS Flash, módulo criptográfico, pilha BLE Mesh, modem celular
 * SIMCom 7663E (PPP), cliente MQTT, e a tarefa central de roteamento/despacho
 * de mensagens criptografadas para a rede local e para a nuvem.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <stdio.h>

#include "ble_mobile.h"
#include "mesh_coordinator.h"
#include "mqtt_publisher.h"
#include "secure_payload.h"
#include "simcom_ppp.h"
#include "stm32_uart.h"
#include "offline_cache.h"
#include "ota_manager.h"
#include "wifi_driver.h"
#include "animal_db.h"

static const char *TAG = "MAIN";

/**
 * @brief Chave simétrica estática padrão para criptografia AES-256 (32 bytes).
 * @warning Para fins de homologação e produção, esta chave deve ser armazenada
 *          de forma protegida em partições NVS criptografadas ou via hardware
 * (eFuse).
 */
static const uint8_t default_aes_key[AES_KEY_SIZE_BYTES] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA,
    0x98, 0x76, 0x54, 0x32, 0x10, 0x10, 0x32, 0x54, 0x76, 0x98, 0xBA,
    0xDC, 0xFE, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};

/**
 * @brief Configuração padrão da APN da operadora celular.
 * @warning Ajustar para a operadora do chip SIM instalado no bastão.
 */
static const simcom_apn_config_t default_apn = {
    .apn = "zap.vivo.com.br",
    .user = "vivo",
    .password = "vivo",
};

/**
 * @brief Configuração padrão do broker MQTT na nuvem.
 * @warning Ajustar URI, tópicos e client_id para o ambiente de produção.
 */
static const mqtt_publisher_config_t default_mqtt_config = {
    .broker_uri = "mqtt://broker.example.com:1883",
    .topic_telemetry = "bastao/telemetria",
    .topic_gps = "bastao/gps",
    .client_id = "bastao-esp-001",
};

/**
 * @brief Task consumidora/despachante encarregada de processar a fila
 * stm32_data_queue.
 * @details Retira dados da fila estruturada preenchida pela recepção UART,
 * serializa-os de volta em JSON padrão, aciona a criptografia AES-256 do módulo
 * secure_payload, e direciona a string criptografada em hexadecimal para a Tela
 * K10 via BLE Mesh.
 *
 * @param[in,out] pvParameters Parâmetros padrão do FreeRTOS (Não utilizado).
 */
static void dispatcher_task(void *pvParameters) {
  stm32_data_t raw_msg;
  char json_buf[320];
  char encrypted_hex[768]; // Buffer suficiente para bloco criptografado em hex
                           // + terminador nulo

  ESP_LOGI(TAG, "Task despachante iniciada com sucesso.");

  while (1) {
    // Aguarda indefinidamente por pacotes vindos da fila UART do STM32
    if (xQueueReceive(stm32_data_queue, &raw_msg, portMAX_DELAY) == pdTRUE) {
      memset(json_buf, 0, sizeof(json_buf));
      memset(encrypted_hex, 0, sizeof(encrypted_hex));

      // 1. Reconstrói o JSON correspondente ao tipo de mensagem
      if (raw_msg.type == DATA_TYPE_RFID) {
        animal_record_t anim_rec;
        esp_err_t db_err = animal_db_lookup(raw_msg.tag, &anim_rec);
        if (db_err == ESP_OK) {
          snprintf(json_buf, sizeof(json_buf),
                   "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\",\"name\":\"%s\",\"weight\":%.2f,\"lot\":\"%s\"}",
                   raw_msg.model, raw_msg.tag, anim_rec.name, anim_rec.weight, anim_rec.lot);
        } else {
          snprintf(json_buf, sizeof(json_buf),
                   "{\"type\":\"rfid\",\"model\":\"%s\",\"tag\":\"%s\"}",
                   raw_msg.model, raw_msg.tag);
        }

        // Notifica o app movel conectado via BLE sobre a nova tag lida
        ble_mobile_notify_tag(json_buf);
        bastao_current_status.tags_read_count++;
      } else if (raw_msg.type == DATA_TYPE_BATTERY) {
        snprintf(json_buf, sizeof(json_buf),
                 "{\"type\":\"batt\",\"volt\":%.2f}", raw_msg.battery_v);

        // Atualiza a tensao de bateria no status global do dispositivo
        bastao_current_status.battery_voltage = raw_msg.battery_v;
      } else {
        ESP_LOGW(TAG, "Mensagem recebida com tipo inválido ou desconhecido.");
        continue;
      }

      ESP_LOGD(TAG, "JSON reconstruído para envio: %s", json_buf);

      // 2. Executa a criptografia AES-256-CBC do JSON
      esp_err_t err = secure_payload_encrypt(json_buf, encrypted_hex,
                                             sizeof(encrypted_hex));
      if (err == ESP_OK) {
        ESP_LOGI(TAG, "Payload criptografado gerado: %s", encrypted_hex);

        // 3. Roteia os dados criptografados para a Tela K10 via BLE Mesh
        err = mesh_coordinator_send_data(encrypted_hex);
        if (err != ESP_OK) {
          ESP_LOGE(TAG, "Falha ao encaminhar dados via BLE Mesh.");
        }

        // 4. Roteia em paralelo para a nuvem via MQTT (SIMCom 7663E)
        err = mqtt_publisher_enqueue(default_mqtt_config.topic_telemetry,
                                     encrypted_hex, 1);
        if (err != ESP_OK) {
          ESP_LOGW(TAG, "Falha ao enfileirar payload para MQTT. Salvando no cache offline...");
          if (offline_cache_write(encrypted_hex) != ESP_OK) {
              ESP_LOGE(TAG, "Falha ao salvar no cache offline.");
          }
        }
      } else {
        ESP_LOGE(TAG, "Falha crítica na criptografia do payload. Código: %d",
                 err);
      }
    }
  }
}

void app_main(void) {
  ESP_LOGI(TAG, "Inicializando o Bastão-ESP Mesh Coordinator...");

  // 1. Inicializa a partição NVS Flash (Necessária para Bluetooth e
  // armazenamento de chaves)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // 1a. Inicializa o banco de dados de animais local
  animal_db_init();

  // 1b. Inicializa o Cache Offline (SPIFFS) e a tarefa de sincronização
  if (offline_cache_init() == ESP_OK) {
    offline_cache_sync_task_start(3);
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar o Cache Offline SPIFFS.");
  }

  // 1c. Inicializa o OTA Manager (cancela rollback se firmware novo rodou com sucesso)
  ota_manager_init();

  // 1d. Inicializa e conecta ao Wi-Fi Station
  if (wifi_driver_init() == ESP_OK) {
    wifi_driver_connect("bastaoIOT", "3spB@st@0");
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar o driver Wi-Fi.");
  }

  // 2. Instancia a fila global compartilhada para tráfego de dados recebidos do
  // STM32
  stm32_data_queue = xQueueCreate(10, sizeof(stm32_data_t));
  if (stm32_data_queue == NULL) {
    ESP_LOGE(TAG,
             "Falha crítica ao criar a fila stm32_data_queue. Abortando...");
    return;
  }

  // 3. Inicializa o módulo criptográfico local com a chave AES-256 padrão
  if (secure_payload_init(default_aes_key) != ESP_OK) {
    ESP_LOGE(TAG, "Falha crítica ao inicializar módulo de segurança.");
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

  // 5. Inicializa o periférico UART1 para interface com o STM32 (Pinos IO13 e
  // IO14)
  if (stm32_uart_init() != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao inicializar o driver serial do STM32.");
  } else {
    // 6. Dispara a tarefa do FreeRTOS encarregada de ler a serial e parsear os
    // JSONs
    if (stm32_uart_rx_task_start(5) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar a task stm32_uart_rx.");
    }
  }

  // 7. Dispara a tarefa despachante com prioridade 6 (superior à task serial
  // para escoamento rápido)
  if (xTaskCreate(dispatcher_task, "dispatcher_task", 4096, NULL, 6, NULL) !=
      pdPASS) {
    ESP_LOGE(TAG, "Falha ao iniciar a task dispatcher_task.");
  }

  // 8b. Inicializa o publicador MQTT (independente da rede ativa)
  if (mqtt_publisher_init(&default_mqtt_config) == ESP_OK) {
    if (mqtt_publisher_task_start(4) != pdPASS) {
      ESP_LOGE(TAG, "Falha ao iniciar task de publicação MQTT.");
    }
  } else {
    ESP_LOGE(TAG, "Falha ao inicializar cliente MQTT.");
  }

  // 8. Inicializa o modem celular SIMCom 7663E e a interface PPP
  ESP_LOGI(TAG, "Inicializando módulo celular SIMCom 7663E...");
  if (simcom_ppp_init() == ESP_OK) {
    if (simcom_ppp_configure_apn(&default_apn) == ESP_OK) {
      if (!wifi_driver_is_connected()) {
        if (simcom_ppp_connect() == ESP_OK) {
          ESP_LOGI(TAG, "Conectividade celular PPP ativa.");
        } else {
          ESP_LOGE(TAG, "Falha inicial ao estabelecer sessão PPP (será tentado pelo Watchdog).");
        }
      } else {
        ESP_LOGI(TAG, "Wi-Fi ativo. Pulando conexão celular inicial.");
      }
    } else {
      ESP_LOGE(TAG, "Falha na configuração da APN.");
    }
  } else {
    ESP_LOGE(TAG, "Falha na inicialização do modem SIMCom.");
  }

  // 10. Inicia o watchdog de reconexão automática do modem (prioridade 3)
  simcom_ppp_watchdog_start(3);

  // Loop principal da task de orquestração do sistema
  // Atualiza periodicamente o status do dispositivo para leituras BLE
  while (1) {
    bastao_current_status.ppp_connected = simcom_ppp_is_connected();
    bastao_current_status.mqtt_connected = mqtt_publisher_is_connected();
    ble_mobile_update_status(&bastao_current_status);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
