/**
 * @file simcom_driver.h
 * @brief Cabecalho do modulo de gerenciamento do modem celular SIMCom via comandos AT.
 * @details Este modulo encapsula toda a comunicacao de baixo nivel com o modem
 *          celular via comandos AT, incluindo registro na rede, controle de GPS,
 *          envio/recepcao de SMS e o uso do cliente MQTT nativo do modem.
 *
 * @author Antigravity Agent
 * @date 2026-06-15
 */

#ifndef SIMCOM_DRIVER_H
#define SIMCOM_DRIVER_H

#include "sdkconfig.h"
#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_publisher.h" // Para definicao de mqtt_publisher_config_t

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definicoes de Configuracao Fisica --- */

/** @brief Porta UART do ESP32 utilizada para comunicacao com o SIMCom 7663E. */
#define SIMCOM_UART_PORT UART_NUM_2

/** @brief Pino GPIO do ESP32 conectado ao TX do modem (ESP32 TX -> Modem RX). */
#define SIMCOM_UART_TX_PIN GPIO_NUM_17

/** @brief Pino GPIO do ESP32 conectado ao RX do modem (ESP32 RX <- Modem TX). */
#define SIMCOM_UART_RX_PIN GPIO_NUM_18

/** @brief Pino GPIO do ESP32 conectado ao PWRKEY do modem SIMCom para boot por hardware. */
#define SIMCOM_PWRKEY_PIN GPIO_NUM_4

/** @brief Baud rate da comunicacao serial com o modem SIMCom. */
#define SIMCOM_UART_BAUD_RATE 115200

/** @brief Tamanho do buffer de recepcao da UART do modem. */
#define SIMCOM_UART_BUF_SIZE 2048

/** @brief Tamanho do buffer de resposta para comandos AT. */
#define AT_RESPONSE_BUF_SIZE 512

/** @brief Tamanho maximo da string de APN. */
#define SIMCOM_APN_MAX_LEN 64

/** @brief Tamanho maximo dos campos de usuario e senha da APN. */
#define SIMCOM_AUTH_MAX_LEN 32

/* --- Estruturas de Dados --- */

/**
 * @brief Estrutura de configuracao da APN da operadora celular.
 */
typedef struct {
    char apn[SIMCOM_APN_MAX_LEN];       /**< Nome da APN (ex: "zap.vivo.com.br") */
    char user[SIMCOM_AUTH_MAX_LEN];      /**< Usuario da APN (vazio se nao necessario) */
    char password[SIMCOM_AUTH_MAX_LEN];  /**< Senha da APN (vazio se nao necessario) */
} simcom_apn_config_t;

/**
 * @brief Estrutura contendo dados de localizacao GPS.
 */
typedef struct {
    bool valid;        /**< Flag indicando se a posicao e valida */
    double latitude;   /**< Latitude decimal (positivo = Norte, negativo = Sul) */
    double longitude;  /**< Longitude decimal (positivo = Leste, negativo = Oeste) */
    float altitude;    /**< Altitude em metros acima do nivel do mar */
    float speed_kmh;   /**< Velocidade em km/h */
} simcom_gps_data_t;

/**
 * @brief Enumeracao dos estados operacionais do modem.
 */
typedef enum {
    SIMCOM_STATE_OFF,               /**< Modem desligado ou nao inicializado */
    SIMCOM_STATE_AT_READY,          /**< Modem respondendo a comandos AT */
    SIMCOM_STATE_REGISTERED,        /**< Modem registrado na rede celular e APN ativa */
    SIMCOM_STATE_MQTT_CONNECTED,    /**< Cliente MQTT integrado conectado ao Broker */
    SIMCOM_STATE_ERROR              /**< Estado de erro ou falha de comunicacao */
} simcom_state_t;

/**
 * @brief Enumeracao das tecnologias de rede celular suportadas.
 */
typedef enum {
    CELLULAR_TECH_NONE = 0,   /**< Sem rede */
    CELLULAR_TECH_2G,         /**< GSM/GPRS 2G */
    CELLULAR_TECH_3G,         /**< UMTS/HSPA 3G */
    CELLULAR_TECH_4G_LTE,     /**< LTE 4G */
    CELLULAR_TECH_NB_IOT,     /**< NB-IoT (LTE Cat NB1/NB2) */
    CELLULAR_TECH_UNKNOWN     /**< Tecnologia desconhecida */
} cellular_tech_t;

/**
 * @brief Estrutura contendo o status completo da rede celular.
 */
typedef struct {
    int rssi;                  /**< RSSI em dBm (-113 a -51) ou 99 se nao detectavel */
    int ber;                   /**< BER (Bit Error Rate) ou -1 se nao disponivel */
    cellular_tech_t tech;      /**< Tecnologia de rede atual */
    int mcc;                   /**< Codigo do pais movel (Mobile Country Code) */
    int mnc;                   /**< Codigo da rede movel (Mobile Network Code) */
    int lac;                   /**< Codigo da area de localizacao (Location Area Code) */
    int cid;                   /**< Cell ID (identificador da celula) */
    char operator_name[32];    /**< Nome da operadora */
    bool registered;          /**< Flag indicando registro na rede */
    bool roaming;             /**< Flag indicando roaming */
    uint32_t tx_bytes;         /**< Reservado para estatisticas (Bytes transmitidos) */
    uint32_t rx_bytes;         /**< Reservado para estatisticas (Bytes recebidos) */
    simcom_state_t modem_state;/**< Estado operacional do modem */
    int rsrp;                  /**< RSRP em dBm (LTE) */
    int rsrq;                  /**< RSRQ em dB (LTE) */
    int sinr;                  /**< SINR/RSSNR em dB (LTE) */
    char ceer[64];             /**< Log de erro estendido (AT+CEER) */
} cellular_status_t;

/* --- Funcoes de Interface Publica --- */

/**
 * @brief Inicializa o driver UART para comunicacao com o modem SIMCom.
 * @details Configura a porta serial UART2, instala o driver e ativa a task
 *          de recebimento e o watchdog celular.
 *
 * @return esp_err_t ESP_OK se o modem responder ao comando AT inicial.
 */
esp_err_t simcom_driver_init(void);

/**
 * @brief Configura a APN da operadora celular e registra na rede.
 *
 * @param[in] apn_config Ponteiro para a estrutura com dados da APN.
 * @return esp_err_t ESP_OK se o registro na rede for bem-sucedido.
 */
esp_err_t simcom_driver_configure_apn(const simcom_apn_config_t *apn_config);

/**
 * @brief Conecta o cliente MQTT integrado do modem ao Broker.
 * @details Configura o Will Topic/Msg, registra o Client ID e estabelece a sessao TCP/MQTT.
 *          Inscreve o modem nos topicos de comando e configuracao do bastao.
 *
 * @param[in] mqtt_config Ponteiro para a estrutura com parametros de conexao MQTT.
 * @return esp_err_t ESP_OK se conectado e inscrito com sucesso.
 */
esp_err_t simcom_driver_mqtt_connect(const mqtt_publisher_config_t *mqtt_config);

/**
 * @brief Encerra a conexao MQTT e para o serviço MQTT do modem.
 * @return esp_err_t ESP_OK se desconectado.
 */
esp_err_t simcom_driver_mqtt_disconnect(void);

/**
 * @brief Publica uma mensagem em um topico usando o motor MQTT integrado do SIMCom.
 *
 * @param[in] topic Topico de destino.
 * @param[in] payload Payload em formato de string.
 * @param[in] qos Nivel de qualidade de servico (QoS).
 * @return esp_err_t ESP_OK se a publicacao foi confirmada.
 */
esp_err_t simcom_driver_mqtt_publish(const char *topic, const char *payload, uint8_t qos);

/**
 * @brief Verifica se o driver SIMCom esta ocupado (mutex em uso por outro comando AT).
 * @return true se ocupado, false se disponivel para publicar.
 */
bool simcom_driver_is_busy(void);

/**
 * @brief Retorna o ultimo RSSI em cache (sem enviar comando AT ao modem).
 * @details O cache e atualizado pelo watchdog (a cada 10s) e por
 *          simcom_driver_get_signal_quality(). Seguro chamar de qualquer task.
 * @return RSSI em dBm, ou 99 se ainda nao medido.
 */
int simcom_driver_get_cached_rssi(void);

/**
 * @brief Retorna o estado operacional atual do modem.
 *
 * @return simcom_state_t Estado atual conforme a enumeracao simcom_state_t.
 */
simcom_state_t simcom_driver_get_state(void);

/**
 * @brief Verifica se a conexao MQTT do modem celular esta ativa.
 *
 * @return true se o MQTT celular estiver conectado.
 */
bool simcom_driver_is_connected(void);

/**
 * @brief Adquire os dados de posicao GPS/GLONASS do modulo SIMCom.
 *
 * @param[out] gps_out Ponteiro para a estrutura que recebera os dados GPS.
 * @return esp_err_t ESP_OK se os dados foram lidos com sucesso.
 */
esp_err_t simcom_driver_get_gps(simcom_gps_data_t *gps_out);

/**
 * @brief Liga o GNSS do modem.
 * @return esp_err_t ESP_OK se o GNSS foi ligado com sucesso.
 */
esp_err_t simcom_driver_gps_power_on(void);

/**
 * @brief Desliga o GNSS do modem para economia de energia.
 * @return esp_err_t ESP_OK se o GNSS foi desligado.
 */
esp_err_t simcom_driver_gps_power_off(void);

/**
 * @brief Configura as constelacoes GNSS (GPS, GLONASS, BDS, Galileo).
 * @details Envia AT+CGNSSMODE=7 (GPS+BDS+GLONASS) para maximizar numero de satelites.
 *          Se o modem nao suportar modo 7, fallback para modo 3 (GPS+QZSS).
 *          Deve ser chamado apos o GNSS estar ligado e pronto (+CGNSSPWR:READY!).
 * @return esp_err_t ESP_OK se configurado.
 */
esp_err_t simcom_driver_configure_gnss(void);

/**
 * @brief Baixa dados de assistencia GNSS (A-GPS) do servidor AGNSS via 4G.
 * @details Sequencia completa:
 *          1. Liga GNSS se nao estiver ativo
 *          2. Aguarda chip pronto (+CGNSSPWR:READY!, ~9s no ASR1601)
 *          3. Configura constelacoes (GPS+BDS+GLONASS)
 *          4. Envia AT+CAGPS para baixar efeméride/almanac via socket TCP
 *          5. Executa AT+CGPSCOLD para reiniciar GPS com dados de assistencia
 *
 *          Com dados AGNSS, o fix GPS deve ocorrer em 2-5s em vez de 30s+.
 *          Requer conexao 4G ativa (PDP context configurado).
 *
 * @return esp_err_t ESP_OK se dados baixados com sucesso.
 */
esp_err_t simcom_driver_download_agps(void);

/**
 * @brief Inicia a task de monitoramento e reconexao automatica do modem.
 *
 * @param[in] priority Prioridade da tarefa FreeRTOS.
 * @return BaseType_t pdPASS se a tarefa foi criada.
 */
BaseType_t simcom_driver_watchdog_start(UBaseType_t priority);

/**
 * @brief Configura se a conectividade celular esta suspensa (ex: quando Wi-Fi esta ativo).
 * 
 * @param[in] suspend true para suspender, false para permitir conexao normal.
 */
void simcom_driver_set_suspended(bool suspend);

/**
 * @brief Retorna o status de suspensao da conexao celular.
 * 
 * @return true se a conectividade celular estiver suspensa.
 */
bool simcom_driver_is_suspended(void);

/**
 * @brief Le a qualidade do sinal celular (RSSI e BER).
 *
 * @param[out] rssi Ponteiro para receber o valor RSSI em dBm.
 * @param[out] ber Ponteiro para receber o valor BER.
 * @return esp_err_t ESP_OK se os dados foram obtidos.
 */
esp_err_t simcom_driver_get_signal_quality(int *rssi, int *ber);

/**
 * @brief Obtem informacoes detalhadas sobre a rede celular conectada.
 */
esp_err_t simcom_driver_get_network_info(cellular_tech_t *tech, int *mcc,
                                         int *mnc, char *operator_name);

/**
 * @brief Obtem o status completo da rede celular para diagnostico.
 */
esp_err_t simcom_driver_get_status(cellular_status_t *status);

/**
 * @brief Gera um JSON com o diagnostico completo da rede celular.
 */
esp_err_t simcom_driver_diagnostic_json(char *json_buf, size_t buf_size);

/**
 * @brief Obtem o IMEI do modem celular.
 */
const char* simcom_driver_get_imei(void);

/**
 * @brief Obtem o numero de telefone (MSISDN) do chip SIM ativo.
 * @param[out] msisdn Buffer para receber o numero (ex: "+5511999999999").
 * @param[in] max_len Tamanho do buffer.
 * @return esp_err_t ESP_OK se o numero foi lido, ESP_ERR_NOT_FOUND se ausente.
 */
esp_err_t simcom_driver_get_msisdn(char *msisdn, size_t max_len);

/**
 * @brief Sincroniza o relogio do sistema ESP32 com a hora da torre celular.
 */
esp_err_t simcom_driver_sync_time_from_tower(void);

/**
 * @brief Seleciona o slot de chip SIM (DSSS - Dual SIM Single Standby).
 */
esp_err_t simcom_driver_select_sim(int slot);

/**
 * @brief Envia uma mensagem SMS.
 */
esp_err_t simcom_driver_send_sms(const char *phone_number, const char *message);

/**
 * @brief Verifica o recebimento de SMS e processa comandos pendentes.
 */
esp_err_t simcom_driver_check_and_process_sms(void);

/**
 * @brief Verifica se o dispositivo tem localizacao disponivel (GPS ou torre celular).
 * @return true se GPS fix ou torre celular valida
 */
bool simcom_driver_has_location(void);

/**
 * @brief Obtem localizacao approximada via torre celular (Mozilla Location Service).
 * @param[out] lat Latitude
 * @param[out] lon Longitude
 * @return ESP_OK se obteve localizacao
 */
esp_err_t simcom_driver_get_cell_tower_location(double *lat, double *lon);

/**
 * @brief Verifica se os dados da torre celular estao validos.
 * @return true se TAC/CID foram extraidos com sucesso
 */
bool simcom_driver_is_cell_tower_valid(void);

#ifdef __cplusplus
}
#endif

#endif /* SIMCOM_DRIVER_H */
