/**
 * @file mqtt_publisher.h
 * @brief Cabecalho do modulo de publicacao MQTT para envio de telemetria a
 *        nuvem.
 * @details Este modulo gerencia o ciclo de vida do cliente MQTT nativo do
 *          ESP-IDF, operando sobre a interface de rede PPP fornecida pelo
 *          modem SIMCom 7663E, e expoe funcoes para publicacao de payloads
 *          criptografados e dados de localizacao GPS.
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

/* --- Definicoes de Configuracao --- */

/** @brief Tamanho maximo da URL do broker MQTT. */
#define MQTT_BROKER_URI_MAX_LEN 128

/** @brief Tamanho maximo do topico MQTT. */
#define MQTT_TOPIC_MAX_LEN 64

/** @brief Tamanho maximo do payload MQTT (hex cifrado). */
#define MQTT_PAYLOAD_MAX_LEN 512

/** @brief Profundidade da fila de publicacao MQTT. */
#define MQTT_PUBLISH_QUEUE_DEPTH 20

/* --- Estruturas de Dados --- */

/**
 * @brief Estrutura de configuracao do cliente MQTT.
 */
typedef struct {
  char broker_uri[MQTT_BROKER_URI_MAX_LEN]; /**< URI do broker
                                                 (ex: "mqtts://broker.com:8883")
                                             */
  char topic_telemetry[MQTT_TOPIC_MAX_LEN]; /**< Topico para dados de
                                                 telemetria RFID/bateria */
  char topic_gps[MQTT_TOPIC_MAX_LEN];       /**< Topico para dados de
                                                 localizacao GPS */
  char client_id[32];                       /**< Identificador unico do
                                                 bastao no broker */
  char username[64]; /**< Usuario para autenticacao MQTT */
  char password[128]; /**< Senha para autenticacao MQTT (AES key hex = 64 chars + null) */
} mqtt_publisher_config_t;

/**
 * @brief Estrutura de uma mensagem pendente na fila de publicacao.
 */
typedef struct {
  char topic[MQTT_TOPIC_MAX_LEN];     /**< Topico de destino */
  char payload[MQTT_PAYLOAD_MAX_LEN]; /**< Payload criptografado em hex */
  uint8_t qos;                        /**< Nivel de QoS MQTT (0, 1 ou 2) */
} mqtt_publish_msg_t;

/* --- Variaveis Globais --- */

/**
 * @brief Handle global da fila de mensagens MQTT pendentes de publicacao.
 * @details O despachante do main.c insere mensagens criptografadas nesta fila
 *          e a task de publicacao as consome de forma assincrona.
 */
extern QueueHandle_t mqtt_publish_queue;

/* --- Funcoes de Interface Publica --- */

/**
 * @brief Inicializa o cliente MQTT nativo do ESP-IDF.
 * @details Cria o handle do cliente MQTT, registra os handlers de eventos
 *          (conexao, desconexao, publicacao) e prepara a fila de publicacao.
 *
 * @param[in] config Ponteiro para a estrutura de configuracao do broker MQTT.
 *
 * @pre A interface de rede PPP deve estar ativa (simcom_ppp_connect ok).
 * @post O cliente MQTT esta instanciado e tenta a conexao ao broker.
 *
 * @return esp_err_t ESP_OK se o cliente foi criado e a conexao iniciada.
 */
esp_err_t mqtt_publisher_init(const mqtt_publisher_config_t *config);

/**
 * @brief Atualiza a configuracao do cliente MQTT em tempo de execucao.
 * @details Para o cliente existente, limpa os recursos e reinicia com a nova config.
 * 
 * @param[in] config Ponteiro para a nova configuracao do broker MQTT.
 * @return esp_err_t ESP_OK se atualizou e reiniciou com sucesso.
 */
esp_err_t mqtt_publisher_update_config(const mqtt_publisher_config_t *config);

/**
 * @brief Cria e inicia a tarefa FreeRTOS de publicacao MQTT.
 * @details A task consome mensagens da fila mqtt_publish_queue e as publica
 *          no broker MQTT de forma assincrona.
 *
 * @param[in] priority Prioridade de execucao da task FreeRTOS.
 *
 * @pre O cliente MQTT deve estar inicializado (mqtt_publisher_init ok).
 *
 * @return BaseType_t pdPASS se a task foi criada com sucesso.
 */
BaseType_t mqtt_publisher_task_start(UBaseType_t priority);

/**
 * @brief Verifica se o cliente MQTT esta conectado ao broker.
 *
 * @return true se a conexao MQTT estiver ativa.
 */
bool mqtt_publisher_is_connected(void);

/**
 * @brief Insere um payload criptografado na fila de publicacao MQTT.
 * @details Funcao auxiliar thread-safe para ser chamada pelo despachante.
 *
 * @param[in] topic Topico MQTT de destino.
 * @param[in] payload String hexadecimal do payload criptografado.
 * @param[in] qos Nivel de qualidade de servico (0, 1 ou 2).
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
