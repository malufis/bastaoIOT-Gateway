/**
 * @file mqtt_publisher.c
 * @brief Implementação do módulo de publicação MQTT para envio de telemetria.
 * @details Este módulo contém o ciclo de vida do cliente MQTT (conexão,
 *          reconexão, handlers de eventos) e a task de publicação que consome
 *          a fila de mensagens pendentes e as envia ao broker MQTT.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "mqtt_publisher.h"
#include <string.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "offline_cache.h"
#include "cJSON.h"
#include "ota_manager.h"
#include "cmd_parser.h"

static const char *TAG = "MQTT_PUB";

/* --- Variáveis Estáticas --- */

/** @brief Handle do cliente MQTT nativo do ESP-IDF. */
static esp_mqtt_client_handle_t mqtt_client = NULL;

/** @brief Flag indicando se o cliente está conectado ao broker. */
static bool mqtt_connected = false;

/** @brief Configuração armazenada localmente para uso pela task. */
static mqtt_publisher_config_t stored_config = {0};

/* --- Definição da Fila Global --- */
QueueHandle_t mqtt_publish_queue = NULL;

/* --- Funções Internas Privadas --- */

/**
 * @brief Handler de eventos MQTT do ESP-IDF.
 * @details Manipula os eventos de ciclo de vida do cliente MQTT, como
 *          conexão, desconexão, publicação confirmada e erros.
 *
 * @param[in] handler_args Argumentos adicionais do handler (não utilizado).
 * @param[in] base Base do evento (MQTT_EVENTS).
 * @param[in] event_id Identificador do evento.
 * @param[in] event_data Dados específicos do evento.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                                int32_t event_id, void *event_data) {
  esp_mqtt_event_handle_t event = event_data;

  switch ((esp_mqtt_event_id_t)event_id) {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG,
             "Conectado ao Broker MQTT com sucesso! (Session Present: %d)",
             event->session_present);
    mqtt_connected = true;
    esp_mqtt_client_subscribe(mqtt_client, "bastao/cmd", 1);
    ESP_LOGI(TAG, "Inscrito no tópico de comandos bastao/cmd");
    break;

  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGW(TAG, "Desconectado do Broker MQTT.");
    mqtt_connected = false;
    break;

  case MQTT_EVENT_PUBLISHED:
    ESP_LOGD(TAG, "Mensagem publicada com sucesso. MSG_ID=%d",
             event->msg_id);
    break;

  case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "Dado recebido no tópico: %.*s", event->topic_len,
             event->topic);
    ESP_LOGI(TAG, "Payload: %.*s", event->data_len, event->data);
    
    // Tratamento de comandos recebidos
    if (strncmp(event->topic, "bastao/cmd", event->topic_len) == 0) {
        char *temp_buf = malloc(event->data_len + 1);
        if (temp_buf != NULL) {
            memcpy(temp_buf, event->data, event->data_len);
            temp_buf[event->data_len] = '\0';

            cmd_parser_process_message(event->topic, temp_buf);
            free(temp_buf);
        }
    }
    break;

  case MQTT_EVENT_ERROR:
    ESP_LOGE(TAG, "Erro MQTT detectado.");
    if (event->error_handle->error_type ==
        MQTT_ERROR_TYPE_TCP_TRANSPORT) {
      ESP_LOGE(TAG, "  Erro de transporte TCP: 0x%x",
               event->error_handle->esp_transport_sock_errno);
    }
    break;

  default:
    ESP_LOGD(TAG, "Evento MQTT não tratado: %" PRIi32, event_id);
    break;
  }
}

/**
 * @brief Task FreeRTOS de publicação MQTT.
 * @details Consome mensagens da fila mqtt_publish_queue e as publica no
 *          broker MQTT. Se o cliente estiver desconectado, descarta a
 *          mensagem com log de aviso.
 *
 * @param[in] pvParameters Parâmetros padrão do FreeRTOS (não utilizado).
 *
 * @note Em uma implementação futura, as mensagens devem ser redirecionadas
 *       para o spooler de cache offline ao invés de descartadas.
 */
static void mqtt_publish_task(void *pvParameters) {
  mqtt_publish_msg_t msg;

  ESP_LOGI(TAG, "Task de publicação MQTT iniciada.");

  while (1) {
    // Aguarda indefinidamente por mensagens na fila de publicação
    if (xQueueReceive(mqtt_publish_queue, &msg, portMAX_DELAY) ==
        pdTRUE) {
      if (!mqtt_connected) {
        ESP_LOGW(
            TAG,
            "MQTT desconectado. Salvando no cache offline (tópico: %s).",
            msg.topic);
        offline_cache_write(msg.payload);
        continue;
      }

      // Publica a mensagem no broker
      int msg_id = esp_mqtt_client_publish(mqtt_client, msg.topic,
                                           msg.payload,
                                           strlen(msg.payload),
                                           msg.qos, 0);
      if (msg_id < 0) {
        ESP_LOGE(TAG,
                 "Falha na publicação MQTT. Tópico: %s. Salvando no cache offline.",
                 msg.topic);
        offline_cache_write(msg.payload);
      } else {
        ESP_LOGI(TAG,
                 "Publicação MQTT agendada. Tópico: %s, MSG_ID: %d",
                 msg.topic, msg_id);
      }
    }
  }
}

/* --- Implementação de Funções Públicas --- */

esp_err_t mqtt_publisher_init(const mqtt_publisher_config_t *config) {
  if (config == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  // Armazena a configuração localmente
  memcpy(&stored_config, config, sizeof(mqtt_publisher_config_t));

  ESP_LOGI(TAG, "Inicializando cliente MQTT para broker: %s",
           config->broker_uri);

  // 1. Cria a fila de publicação MQTT
  if (mqtt_publish_queue == NULL) {
    mqtt_publish_queue =
        xQueueCreate(MQTT_PUBLISH_QUEUE_DEPTH, sizeof(mqtt_publish_msg_t));
    if (mqtt_publish_queue == NULL) {
      ESP_LOGE(TAG, "Falha ao criar fila de publicação MQTT.");
      return ESP_ERR_NO_MEM;
    }
  }

  // 2. Configura o cliente MQTT
  esp_mqtt_client_config_t mqtt_cfg = {
      .broker.address.uri = config->broker_uri,
      .credentials.client_id = config->client_id,
      .session.keepalive = 120,
      .network.reconnect_timeout_ms = 10000,
  };

  mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
  if (mqtt_client == NULL) {
    ESP_LOGE(TAG, "Falha ao instanciar o cliente MQTT.");
    return ESP_FAIL;
  }

  // 3. Registra o handler de eventos MQTT
  esp_err_t err = esp_mqtt_client_register_event(
      mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao registrar handler de eventos MQTT: %d",
             err);
    return err;
  }

  // 4. Inicia o cliente MQTT (conexão é assíncrona)
  err = esp_mqtt_client_start(mqtt_client);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Falha ao iniciar cliente MQTT: %d", err);
    return err;
  }

  ESP_LOGI(TAG,
           "Cliente MQTT iniciado. Conexão ao broker em andamento...");
  return ESP_OK;
}

BaseType_t mqtt_publisher_task_start(UBaseType_t priority) {
  return xTaskCreate(mqtt_publish_task, "mqtt_pub_task", 4096, NULL,
                     priority, NULL);
}

bool mqtt_publisher_is_connected(void) { return mqtt_connected; }

esp_err_t mqtt_publisher_enqueue(const char *topic, const char *payload,
                                 uint8_t qos) {
  if (topic == NULL || payload == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  if (mqtt_publish_queue == NULL) {
    ESP_LOGE(TAG, "Fila de publicação MQTT não inicializada.");
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
