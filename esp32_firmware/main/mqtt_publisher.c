/**
 * @file mqtt_publisher.c
 * @brief Implementacao do modulo de publicacao MQTT para envio de telemetria.
 * @details Este modulo contem o ciclo de vida do cliente MQTT (conexao,
 *          reconexao, handlers de eventos) e a task de publicacao que consome
 *          a fila de mensagens pendentes e as envia ao broker MQTT.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "mqtt_publisher.h"
#include "cJSON.h"
#include "cmd_parser.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "offline_cache.h"
#include "ota_manager.h"
#include "simcom_ppp.h"
#include "esp_mac.h"
#include <string.h>
#include "ble_mobile.h"


static const char *TAG = "MQTT_PUB";

/* --- Variaveis Estaticas --- */

/** @brief Handle do cliente MQTT nativo do ESP-IDF. */
static esp_mqtt_client_handle_t mqtt_client = NULL;

/** @brief Flag indicando se o cliente esta conectado ao broker. */
static bool mqtt_connected = false;

/* --- Topicos Dinamicos baseados no IMEI --- */
static char dynamic_topic_telemetry[128] = {0};
static char dynamic_topic_gps[128] = {0};
static char dynamic_topic_cmd[128] = {0};
static char dynamic_topic_config[128] = {0};

static void init_dynamic_topics(void) {
  if (dynamic_topic_telemetry[0] == '\0') {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char mac_str[14];
    snprintf(mac_str, sizeof(mac_str), "%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    snprintf(dynamic_topic_telemetry, sizeof(dynamic_topic_telemetry), "agro/bastao/%s/telemetry", mac_str);
    snprintf(dynamic_topic_gps, sizeof(dynamic_topic_gps), "agro/bastao/%s/gps", mac_str);
    snprintf(dynamic_topic_cmd, sizeof(dynamic_topic_cmd), "id/%s/cmd", mac_str);
    snprintf(dynamic_topic_config, sizeof(dynamic_topic_config), "id/%s/config", mac_str);
    ESP_LOGI(TAG, "Topicos dinamicos gerados com MAC %s: Telemetria=%s, GPS=%s, Comando=%s, Config=%s",
             mac_str, dynamic_topic_telemetry, dynamic_topic_gps, dynamic_topic_cmd, dynamic_topic_config);
  }
}

/** @brief Configuracao armazenada localmente para uso pela task. */
static mqtt_publisher_config_t stored_config = {0};

/* --- Definicao da Fila Global --- */
QueueHandle_t mqtt_publish_queue = NULL;

/* --- Funcoes Internas Privadas --- */

/**
 * @brief Handler de eventos MQTT do ESP-IDF.
 * @details Manipula os eventos de ciclo de vida do cliente MQTT, como
 *          conexao, desconexao, publicacao confirmada e erros.
 *
 * @param[in] handler_args Argumentos adicionais do handler (nao utilizado).
 * @param[in] base Base do evento (MQTT_EVENTS).
 * @param[in] event_id Identificador do evento.
 * @param[in] event_data Dados especificos do evento.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
  esp_mqtt_event_handle_t event = event_data;

  switch ((esp_mqtt_event_id_t)event_id) {
  case MQTT_EVENT_CONNECTED:
    init_dynamic_topics();
    ESP_LOGI(TAG, "Conectado ao Broker MQTT com sucesso! (Session Present: %d)",
             event->session_present);
    mqtt_connected = true;
    esp_mqtt_client_subscribe(mqtt_client, dynamic_topic_cmd, 1);
    ESP_LOGI(TAG, "Inscrito no topico de comandos: %s", dynamic_topic_cmd);
    esp_mqtt_client_subscribe(mqtt_client, dynamic_topic_config, 1);
    ESP_LOGI(TAG, "Inscrito no topico de configuracao: %s", dynamic_topic_config);
    break;

  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGW(TAG, "Desconectado do Broker MQTT.");
    mqtt_connected = false;
    break;

  case MQTT_EVENT_PUBLISHED:
    ESP_LOGD(TAG, "Mensagem publicada com sucesso. MSG_ID=%d", event->msg_id);
    break;

  case MQTT_EVENT_DATA:
    init_dynamic_topics();
    ESP_LOGI(TAG, "Dado recebido no topico: %.*s", event->topic_len,
             event->topic);
    ESP_LOGI(TAG, "Payload: %.*s", event->data_len, event->data);

    // Tratamento de comandos recebidos
    if (event->topic_len == strlen(dynamic_topic_cmd) && 
        strncmp(event->topic, dynamic_topic_cmd, event->topic_len) == 0) {
      char *temp_buf = malloc(event->data_len + 1);
      if (temp_buf != NULL) {
        memcpy(temp_buf, event->data, event->data_len);
        temp_buf[event->data_len] = '\0';

        cmd_parser_process_message(event->topic, temp_buf);
        free(temp_buf);
      }
    }
    // Tratamento de configuracoes recebidas
    else if (event->topic_len == strlen(dynamic_topic_config) &&
             strncmp(event->topic, dynamic_topic_config, event->topic_len) == 0) {
      char *temp_buf = malloc(event->data_len + 1);
      if (temp_buf != NULL) {
        memcpy(temp_buf, event->data, event->data_len);
        temp_buf[event->data_len] = '\0';

        ble_mobile_process_config_json(temp_buf);
        free(temp_buf);
      }
    }
    break;

  case MQTT_EVENT_ERROR:
    ESP_LOGE(TAG, "Erro MQTT detectado.");
    if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
      ESP_LOGE(TAG, "  Erro de transporte TCP: 0x%x",
               event->error_handle->esp_transport_sock_errno);
    }
    break;

  default:
    ESP_LOGD(TAG, "Evento MQTT nao tratado: %" PRIi32, event_id);
    break;
  }
}

/**
 * @brief Task FreeRTOS de publicacao MQTT.
 * @details Consome mensagens da fila mqtt_publish_queue e as publica no
 *          broker MQTT. Se o cliente estiver desconectado, descarta a
 *          mensagem com log de aviso.
 *
 * @param[in] pvParameters Parametros padrao do FreeRTOS (nao utilizado).
 *
 * @note Em uma implementacao futura, as mensagens devem ser redirecionadas
 *       para o spooler de cache offline ao inves de descartadas.
 */
static void mqtt_publish_task(void *pvParameters) {
  mqtt_publish_msg_t msg;

  ESP_LOGI(TAG, "Task de publicacao MQTT iniciada.");

  while (1) {
    // Aguarda indefinidamente por mensagens na fila de publicacao
    if (xQueueReceive(mqtt_publish_queue, &msg, portMAX_DELAY) == pdTRUE) {
      if (!mqtt_connected) {
        ESP_LOGW(TAG,
                 "MQTT desconectado. Salvando no cache offline (topico: %s).",
                 msg.topic);
        offline_cache_write(msg.payload);
        continue;
      }

      // Traduz topicos estaticos para dinamicos baseados no IMEI
      const char *publish_topic = msg.topic;
      init_dynamic_topics();
      if (strcmp(msg.topic, stored_config.topic_telemetry) == 0 || strstr(msg.topic, "telemetria") != NULL) {
        publish_topic = dynamic_topic_telemetry;
      } else if (strcmp(msg.topic, stored_config.topic_gps) == 0 || strstr(msg.topic, "gps") != NULL) {
        publish_topic = dynamic_topic_gps;
      }

      // Publica a mensagem no broker
      int msg_id = esp_mqtt_client_publish(mqtt_client, publish_topic, msg.payload,
                                           strlen(msg.payload), msg.qos, 0);
      if (msg_id < 0) {
        ESP_LOGE(
            TAG,
            "Falha na publicacao MQTT. Topico: %s. Salvando no cache offline.",
            publish_topic);
        offline_cache_write(msg.payload);
      } else {
        ESP_LOGI(TAG, "Publicacao MQTT agendada. Topico: %s, MSG_ID: %d",
                 publish_topic, msg_id);
      }
    }
  }
}

/* --- Implementacao de Funcoes Publicas --- */

esp_err_t mqtt_publisher_init(const mqtt_publisher_config_t *config) {
  if (config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  // Armazena a configuracao localmente
  memcpy(&stored_config, config, sizeof(mqtt_publisher_config_t));

  ESP_LOGI(TAG, "Inicializando cliente MQTT para broker: %s",
           config->broker_uri);

  // 1. Cria a fila de publicacao MQTT
  if (mqtt_publish_queue == NULL) {
    mqtt_publish_queue =
        xQueueCreate(MQTT_PUBLISH_QUEUE_DEPTH, sizeof(mqtt_publish_msg_t));
    if (mqtt_publish_queue == NULL) {
      ESP_LOGE(TAG, "Falha ao criar fila de publicacao MQTT.");
      return ESP_ERR_NO_MEM;
    }
  }

  // 2. Gera client_id unico dinamico com base no IMEI ou MAC
  char dynamic_client_id[64];
  const char *imei = simcom_ppp_get_imei();
  if (imei != NULL && strcmp(imei, "bastao-esp-default") != 0 && strlen(imei) > 0) {
    snprintf(dynamic_client_id, sizeof(dynamic_client_id), "bastao_%s", imei);
  } else {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(dynamic_client_id, sizeof(dynamic_client_id), "bastao_%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }

  // 3. Configura o cliente MQTT
  esp_mqtt_client_config_t mqtt_cfg = {
      .broker.address.uri = config->broker_uri,
      .credentials.client_id = dynamic_client_id,
      .credentials.username = config->username,
      .credentials.authentication.password = config->password,
      .session.keepalive = 60,
      .network.reconnect_timeout_ms = 10000,
  };

  mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
  if (mqtt_client == NULL) {
    ESP_LOGE(TAG, "Falha ao instanciar o cliente MQTT.");
    return ESP_FAIL;
  }

  // 3. Registra o handler de eventos MQTT
  esp_err_t err = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID,
                                                 mqtt_event_handler, NULL);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao registrar handler de eventos MQTT: %d", err);
    return err;
  }

  // 4. Inicia o cliente MQTT (conexao e assincrona)
  err = esp_mqtt_client_start(mqtt_client);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao iniciar cliente MQTT: %d", err);
    return err;
  }

  ESP_LOGI(TAG, "Cliente MQTT iniciado. Conexao ao broker em andamento...");
  return ESP_OK;
}

esp_err_t mqtt_publisher_update_config(const mqtt_publisher_config_t *config) {
  if (config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  ESP_LOGI(TAG, "Atualizando configuracao do cliente MQTT em execucao...");

  // Armazena a configuracao localmente
  memcpy(&stored_config, config, sizeof(mqtt_publisher_config_t));

  if (mqtt_client != NULL) {
    ESP_LOGI(TAG, "Parando e destruindo o cliente MQTT antigo...");
    esp_mqtt_client_stop(mqtt_client);
    esp_mqtt_client_destroy(mqtt_client);
    mqtt_client = NULL;
    mqtt_connected = false;
  }

  // Gera client_id unico dinamico com base no IMEI ou MAC
  char dynamic_client_id[64];
  const char *imei = simcom_ppp_get_imei();
  if (imei != NULL && strcmp(imei, "bastao-esp-default") != 0 && strlen(imei) > 0) {
    snprintf(dynamic_client_id, sizeof(dynamic_client_id), "bastao_%s", imei);
  } else {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(dynamic_client_id, sizeof(dynamic_client_id), "bastao_%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }

  // Configura o cliente MQTT
  esp_mqtt_client_config_t mqtt_cfg = {
      .broker.address.uri = config->broker_uri,
      .credentials.client_id = dynamic_client_id,
      .credentials.username = config->username,
      .credentials.authentication.password = config->password,
      .session.keepalive = 60,
      .network.reconnect_timeout_ms = 10000,
  };

  mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
  if (mqtt_client == NULL) {
    ESP_LOGE(TAG, "Falha ao instanciar o novo cliente MQTT.");
    return ESP_FAIL;
  }

  esp_err_t err = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID,
                                                 mqtt_event_handler, NULL);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao registrar handler de eventos MQTT: %d", err);
    return err;
  }

  err = esp_mqtt_client_start(mqtt_client);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao iniciar novo cliente MQTT: %d", err);
    return err;
  }

  // Forca regeracao dos topicos dinamicos no proximo uso/conexao
  dynamic_topic_telemetry[0] = '\0';
  dynamic_topic_gps[0] = '\0';
  dynamic_topic_cmd[0] = '\0';
  dynamic_topic_config[0] = '\0';

  ESP_LOGI(TAG, "Novo cliente MQTT iniciado e conectando...");
  return ESP_OK;
}

BaseType_t mqtt_publisher_task_start(UBaseType_t priority) {
  return xTaskCreate(mqtt_publish_task, "mqtt_pub_task", 4096, NULL, priority,
                     NULL);
}

bool mqtt_publisher_is_connected(void) { return mqtt_connected; }

esp_err_t mqtt_publisher_enqueue(const char *topic, const char *payload,
                                 uint8_t qos) {
  if (topic == NULL || payload == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  if (mqtt_publish_queue == NULL) {
    ESP_LOGE(TAG, "Fila de publicacao MQTT nao inicializada.");
    return ESP_ERR_INVALID_STATE;
  }

  mqtt_publish_msg_t msg;
  memset(&msg, 0, sizeof(mqtt_publish_msg_t));
  strncpy(msg.topic, topic, sizeof(msg.topic) - 1);
  strncpy(msg.payload, payload, sizeof(msg.payload) - 1);
  msg.qos = qos;

  if (xQueueSend(mqtt_publish_queue, &msg, 0) != pdTRUE) {
    ESP_LOGW(TAG, "Fila MQTT cheia. Mensagem descartada.");
    return ESP_ERR_NO_MEM;
  }

  return ESP_OK;
}
