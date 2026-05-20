/**
 * @file simcom_ppp.h
 * @brief Cabeçalho do módulo de gerenciamento do modem celular SIMCom 7663E.
 * @details Este módulo encapsula toda a comunicação de baixo nível com o modem
 *          celular via comandos AT, a ativação da interface de rede PPP
 *          (Point-to-Point Protocol) integrada ao ESP-NETIF/LwIP, e a
 *          aquisição de dados de localização GPS/GLONASS.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef SIMCOM_PPP_H
#define SIMCOM_PPP_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definições de Configuração Física --- */

/** @brief Porta UART do ESP32 utilizada para comunicação com o SIMCom 7663E. */
#define SIMCOM_UART_PORT UART_NUM_2

/** @brief Pino GPIO do ESP32 conectado ao TX do modem (ESP32 TX -> Modem RX). */
#define SIMCOM_UART_TX_PIN GPIO_NUM_17

/** @brief Pino GPIO do ESP32 conectado ao RX do modem (ESP32 RX <- Modem TX). */
#define SIMCOM_UART_RX_PIN GPIO_NUM_18

/** @brief Baud rate da comunicação serial com o modem SIMCom. */
#define SIMCOM_UART_BAUD_RATE 115200

/** @brief Tamanho do buffer de recepção da UART do modem. */
#define SIMCOM_UART_BUF_SIZE 2048

/** @brief Tamanho máximo da string de APN. */
#define SIMCOM_APN_MAX_LEN 64

/** @brief Tamanho máximo dos campos de usuário e senha da APN. */
#define SIMCOM_AUTH_MAX_LEN 32

/* --- Estruturas de Dados --- */

/**
 * @brief Estrutura de configuração da APN da operadora celular.
 */
typedef struct {
    char apn[SIMCOM_APN_MAX_LEN];       /**< Nome da APN (ex: "zap.vivo.com.br") */
    char user[SIMCOM_AUTH_MAX_LEN];      /**< Usuário da APN (vazio se não necessário) */
    char password[SIMCOM_AUTH_MAX_LEN];  /**< Senha da APN (vazio se não necessário) */
} simcom_apn_config_t;

/**
 * @brief Estrutura contendo dados de localização GPS.
 */
typedef struct {
    bool valid;        /**< Flag indicando se a posição é válida */
    double latitude;   /**< Latitude decimal (positivo = Norte, negativo = Sul) */
    double longitude;  /**< Longitude decimal (positivo = Leste, negativo = Oeste) */
    float altitude;    /**< Altitude em metros acima do nível do mar */
    float speed_kmh;   /**< Velocidade em km/h */
} simcom_gps_data_t;

/**
 * @brief Enumeração dos estados operacionais do modem.
 */
typedef enum {
    SIMCOM_STATE_OFF,          /**< Modem desligado ou não inicializado */
    SIMCOM_STATE_AT_READY,     /**< Modem respondendo a comandos AT */
    SIMCOM_STATE_REGISTERED,   /**< Modem registrado na rede celular */
    SIMCOM_STATE_PPP_ACTIVE,   /**< Interface PPP ativa e com IP alocado */
    SIMCOM_STATE_ERROR         /**< Estado de erro ou falha de comunicação */
} simcom_state_t;

/**
 * @brief Enumeração das tecnologias de rede celular suportadas.
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
    int rssi;                  /**< RSSI em dBm (-113 a -51) ou 99 se não detectável */
    int ber;                   /**< BER (Bit Error Rate) ou -1 se não disponível */
    cellular_tech_t tech;      /**< Tecnologia de rede atual */
    int mcc;                   /**< Código do país móvel (Mobile Country Code) */
    int mnc;                   /**< Código da rede móvel (Mobile Network Code) */
    int lac;                   /**< Código da área de localização (Location Area Code) */
    int cid;                   /**< Cell ID (identificador da célula) */
    char operator_name[32];    /**< Nome da operadora */
    bool registered;          /**< Flag indicando registro na rede */
    bool roaming;             /**< Flag indicando roaming */
    uint32_t tx_bytes;         /**< Bytes transmitidos via PPP */
    uint32_t rx_bytes;         /**< Bytes recebidos via PPP */
    simcom_state_t modem_state;/**< Estado operacional do modem */
} cellular_status_t;

/* --- Funções de Interface Pública --- */

/**
 * @brief Inicializa o driver UART para comunicação com o modem SIMCom 7663E.
 * @details Configura a porta serial UART2 nos pinos 17 (TX) e 18 (RX) do
 *          ESP32, instala o driver de buffer e realiza o teste de eco AT.
 *
 * @pre O hardware do ESP32 deve estar alimentado e os pinos 17/18 livres.
 * @post O driver UART2 está instalado e a comunicação AT está ativa.
 *
 * @return esp_err_t ESP_OK se o modem responder ao comando AT inicial.
 */
esp_err_t simcom_ppp_init(void);

/**
 * @brief Configura a APN da operadora celular e registra na rede 4G/LTE.
 * @details Envia comandos AT para configurar o perfil de dados PDP Context
 *          e aguarda o registro na rede celular (CREG/CGREG).
 *
 * @param[in] apn_config Ponteiro para a estrutura com dados da APN.
 *
 * @pre O modem deve estar inicializado (simcom_ppp_init concluído).
 * @post O modem está registrado na rede celular com APN configurada.
 *
 * @return esp_err_t ESP_OK se o registro na rede for bem-sucedido.
 *                   ESP_ERR_TIMEOUT se o registro demorar além do esperado.
 */
esp_err_t simcom_ppp_configure_apn(const simcom_apn_config_t *apn_config);

/**
 * @brief Ativa a sessão de dados PPP e sobe a interface de rede IP.
 * @details Envia o comando ATD*99# para comutar o modem do modo de comandos
 *          para o modo de dados PPP, e integra a interface de rede ao
 *          stack LwIP do ESP32 via ESP-NETIF.
 *
 * @pre O modem deve estar registrado na rede (simcom_ppp_configure_apn ok).
 * @post Uma interface de rede PPP está ativa no ESP32 com IP válido.
 *
 * @return esp_err_t ESP_OK se a sessão PPP estiver ativa com IP válido.
 */
esp_err_t simcom_ppp_connect(void);

/**
 * @brief Encerra a sessão de dados PPP e retorna ao modo de comandos AT.
 *
 * @pre A sessão PPP deve estar ativa.
 * @post O modem retorna ao modo de comandos AT e a interface de rede é removida.
 *
 * @return esp_err_t ESP_OK se a desconexão foi concluída.
 */
esp_err_t simcom_ppp_disconnect(void);

/**
 * @brief Retorna o estado operacional atual do modem.
 *
 * @return simcom_state_t Estado atual conforme a enumeração simcom_state_t.
 */
simcom_state_t simcom_ppp_get_state(void);

/**
 * @brief Verifica se a interface PPP está ativa e conectada com IP válido.
 *
 * @return true se a interface PPP estiver operacional.
 */
bool simcom_ppp_is_connected(void);

/**
 * @brief Adquire os dados de posição GPS/GLONASS do módulo SIMCom.
 * @details Envia o comando AT+CGPSINFO e decodifica a resposta NMEA
 *          contendo latitude, longitude, altitude e velocidade.
 *
 * @param[out] gps_out Ponteiro para a estrutura que receberá os dados GPS.
 *
 * @pre O modem deve estar no modo de comandos AT (PPP desconectado ou em pausa).
 *
 * @return esp_err_t ESP_OK se os dados GPS foram obtidos com sucesso.
 *                   ESP_ERR_NOT_FOUND se o GPS não tiver fix válido.
 *
 * @note Esta função requer que o modem esteja em modo de comando (não PPP).
 *       Em cenários de operação contínua PPP, é necessário desconectar
 *       temporariamente, obter o GPS, e reconectar.
 */
esp_err_t simcom_ppp_get_gps(simcom_gps_data_t *gps_out);

/**
 * @brief Inicia a task de monitoramento e reconexão automática do modem.
 * @details Cria uma tarefa FreeRTOS que monitora periodicamente o estado
 *          da conexão PPP e reconecta automaticamente em caso de queda.
 *
 * @param[in] priority Prioridade da tarefa FreeRTOS.
 *
 * @return BaseType_t pdPASS se a tarefa foi criada com sucesso.
 */
BaseType_t simcom_ppp_watchdog_start(UBaseType_t priority);

/**
 * @brief Configura se a conectividade celular está suspensa (ex: quando Wi-Fi está ativo).
 * 
 * @param[in] suspend true para suspender e desconectar o modem, false para permitir conexão normal.
 */
void simcom_ppp_set_suspended(bool suspend);

/**
 * @brief Retorna o status de suspensão da conexão celular.
 * 
 * @return true se a conectividade celular estiver suspensa.
 */
bool simcom_ppp_is_suspended(void);

/**
 * @brief Lê a qualidade do sinal celular (RSSI e BER).
 * @details Envia AT+CSQ para obter RSSI (Received Signal Strength Indicator)
 *          e BER (Bit Error Rate).
 *
 * @param[out] rssi Ponteiro para receber o valor RSSI em dBm (-113 a -51, ou 99).
 * @param[out] ber Ponteiro para receber o valor BER (-1 se não disponível).
 *
 * @return esp_err_t ESP_OK se os dados foram obtidos com sucesso.
 */
esp_err_t simcom_ppp_get_signal_quality(int *rssi, int *ber);

/**
 * @brief Obtém informações detalhadas sobre a rede celular conectada.
 * @details Envia AT+QNWINFO para identificar tecnologia, banda e operador.
 *
 * @param[out] tech Ponteiro para receber a tecnologia de rede.
 * @param[out] mcc Ponteiro para receber o código do país (ex: 55 para Brasil).
 * @param[out] mnc Ponteiro para receber o código da rede.
 * @param[out] operator_name Buffer para receber o nome da operadora.
 *
 * @return esp_err_t ESP_OK se as informações foram obtidas com sucesso.
 */
esp_err_t simcom_ppp_get_network_info(cellular_tech_t *tech, int *mcc,
                                       int *mnc, char *operator_name);

/**
 * @brief Obtém o status completo da rede celular para diagnóstico.
 * @details Consolida RSSI, tecnologia, operador, registro e contador de bytes.
 *
 * @param[out] status Ponteiro para a estrutura que receberá o status completo.
 *
 * @return esp_err_t ESP_OK se o status foi obtido com sucesso.
 */
esp_err_t simcom_ppp_get_status(cellular_status_t *status);

/**
 * @brief Gera um JSON com o diagnóstico completo da rede celular.
 * @details Útil para logs de campo e transmissão via MQTT.
 *
 * @param[out] json_buf Buffer para receber o JSON格式.
 * @param[in] buf_size Tamanho do buffer.
 *
 * @return esp_err_t ESP_OK se o JSON foi gerado com sucesso.
 */
esp_err_t simcom_ppp_diagnostic_json(char *json_buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* SIMCOM_PPP_H */
