/**
 * @file mqtt_publisher.h
 * @brief Cabeçalho do módulo de publicação MQTT para envio de telemetria à
 *        nuvem.
 * @details Este módulo gerencia o ciclo de vida do cliente MQTT nativo do
 *          ESP-IDF, operando sobre a interface de rede PPP fornecida pelo
 *          modem SIMCom 7663E, e expõe funções para publicação de payloads
 *          criptografados e dados de localização GPS.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definições de Configuração --- */

/** @brief Tamanho máximo da URL do broker MQTT. */
#define MQTT_BROKER_URI_MAX_LEN 128

/** @brief Tamanho máximo do tópico MQTT. */
#define MQTT_TOPIC_MAX_LEN 64

/** @brief Tamanho máximo do payload MQTT (hex cifrado). */
#define MQTT_PAYLOAD_MAX_LEN 512

/** @brief Profundidade da fila de publicação MQTT. */
#define MQTT_PUBLISH_QUEUE_DEPTH 20

/* --- Estruturas de Dados --- */

/**
 * @brief Estrutura de configuração do cliente MQTT.
 */
typedef struct {
  char broker_uri[MQTT_BROKER_URI_MAX_LEN]; /**< URI do broker
                                                 (ex: "mqtts://broker.com:8883")
                                             */
  char topic_telemetry[MQTT_TOPIC_MAX_LEN]; /**< Tópico para dados de
                                                 telemetria RFID/bateria */
  char topic_gps[MQTT_TOPIC_MAX_LEN];       /**< Tópico para dados de
                                                 localização GPS */
  char client_id[32];                       /**< Identificador único do
                                                 bastão no broker */
} mqtt_publisher_config_t;

/**
 * @brief Estrutura de uma mensagem pendente na fila de publicação.
 */
typedef struct {
  char topic[MQTT_TOPIC_MAX_LEN];     /**< Tópico de destino */
  char payload[MQTT_PAYLOAD_MAX_LEN]; /**< Payload criptografado em hex */
  uint8_t qos;                        /**< Nível de QoS MQTT (0, 1 ou 2) */
} mqtt_publish_msg_t;

/* --- Variáveis Globais --- */

/**
 * @brief Handle global da fila de mensagens MQTT pendentes de publicação.
 * @details O despachante do main.c insere mensagens criptografadas nesta fila
 *          e a task de publicação as consome de forma assíncrona.
 */
extern QueueHandle_t mqtt_publish_queue;

/* --- Funções de Interface Pública --- */

/**
 * @brief Inicializa o cliente MQTT nativo do ESP-IDF.
 * @details Cria o handle do cliente MQTT, registra os handlers de eventos
 *          (conexão, desconexão, publicação) e prepara a fila de publicação.
 *
 * @param[in] config Ponteiro para a estrutura de configuração do broker MQTT.
 *
 * @pre A interface de rede PPP deve estar ativa (simcom_ppp_connect ok).
 * @post O cliente MQTT está instanciado e tenta a conexão ao broker.
 *
 * @return esp_err_t ESP_OK se o cliente foi criado e a conexão iniciada.
 */
esp_err_t mqtt_publisher_init(const mqtt_publisher_config_t *config);

/**
 * @brief Cria e inicia a tarefa FreeRTOS de publicação MQTT.
 * @details A task consome mensagens da fila mqtt_publish_queue e as publica
 *          no broker MQTT de forma assíncrona.
 *
 * @param[in] priority Prioridade de execução da task FreeRTOS.
 *
 * @pre O cliente MQTT deve estar inicializado (mqtt_publisher_init ok).
 *
 * @return BaseType_t pdPASS se a task foi criada com sucesso.
 */
BaseType_t mqtt_publisher_task_start(UBaseType_t priority);

/**
 * @brief Verifica se o cliente MQTT está conectado ao broker.
 *
 * @return true se a conexão MQTT estiver ativa.
 */
bool mqtt_publisher_is_connected(void);

/**
 * @brief Insere um payload criptografado na fila de publicação MQTT.
 * @details Função auxiliar thread-safe para ser chamada pelo despachante.
 *
 * @param[in] topic Tópico MQTT de destino.
 * @param[in] payload String hexadecimal do payload criptografado.
 * @param[in] qos Nível de qualidade de serviço (0, 1 ou 2).
 *
 * @return esp_err_t ESP_OK se a mensagem foi enfileirada com sucesso.
 *                   ESP_ERR_NO_MEM se a fila estiver cheia.
 */
esp_err_t mqtt_publisher_enqueue(const char *topic, const char *payload,
                                 uint8_t qos);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_PUBLISHER_H */
