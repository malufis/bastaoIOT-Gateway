/**
 * @file ble_mobile.h
 * @brief Cabecalho do modulo de comunicacao Bluetooth (BLE) com aplicativo
 *        de celular via GATT Server.
 * @details Este modulo implementa um servidor GATT seguro no ESP32 para
 *          permitir que um aplicativo movel (Android/iOS) sincronize dados
 *          de configuracao do bastao e dados de negocio (Fazenda, Lote, Animal)
 *          de forma bidirecional e criptografada.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#ifndef BLE_MOBILE_H
#define BLE_MOBILE_H

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definicoes de UUIDs do Servico GATT --- */

/**
 * @brief UUID base customizado do servico GATT do Bastao-ESP.
 * @details Formato: XXXXXXXX-BAST-4A0E-8F2D-BASTAO00ESP32
 *          Os primeiros 4 bytes sao substituidos pelo UUID especifico.
 */
#define BASTAO_SERVICE_UUID        0x00FF  /**< UUID do servico principal */
#define CHAR_CONFIG_WRITE_UUID     0xFF01  /**< Config escrita (app -> bastao) */
#define CHAR_CONFIG_READ_UUID      0xFF02  /**< Config leitura (bastao -> app) */
#define CHAR_BUSINESS_WRITE_UUID   0xFF03  /**< Dados de negocio (app -> bastao) */
#define CHAR_LAST_TAG_UUID         0xFF04  /**< Ultima tag lida (notify -> app) */
#define CHAR_DEVICE_STATUS_UUID    0xFF05  /**< Status do dispositivo (read) */

/* --- Tamanhos de Buffer --- */

/** @brief Tamanho maximo do payload de uma caracteristica GATT. */
#define BLE_MOBILE_ATTR_MAX_LEN    512

/** @brief Tamanho maximo do nome do dispositivo BLE. */
#define BLE_MOBILE_DEVICE_NAME_LEN 32

/* --- Chaves NVS para Persistencia --- */

#define NVS_NAMESPACE_CONFIG       "bastao_cfg"   /**< Namespace NVS para config */
#define NVS_NAMESPACE_BUSINESS     "bastao_biz"   /**< Namespace NVS para negocio */
#define NVS_KEY_FARM               "farm_data"    /**< Dados da Fazenda */
#define NVS_KEY_LOT                "lot_data"     /**< Dados do Lote */
#define NVS_KEY_ANIMAL             "animal_data"  /**< Dados do Animal */
#define NVS_KEY_YRM100_POWER       "yrm_power"    /**< Potencia do YRM100 */
#define NVS_KEY_SCAN_TIME          "scan_time"    /**< Tempo de varredura */

/* --- Estruturas de Dados --- */

/**
 * @brief Estrutura representando os parametros de configuracao do bastao.
 */
typedef struct {
    uint8_t yrm100_power;    /**< Potencia de transmissao do YRM100 (0-26 dBm) */
    uint16_t scan_time_ms;   /**< Tempo de varredura de tags em milissegundos */
    uint8_t battery_report_interval_s; /**< Intervalo de telemetria de bateria */
    bool wl134_enabled;      /**< Habilitar/desabilitar leitor LF WL-134 */
    bool yrm100_enabled;     /**< Habilitar/desabilitar leitor UHF YRM100 */
} bastao_config_t;

/**
 * @brief Estrutura representando o status atual do dispositivo.
 */
typedef struct {
    float battery_voltage;   /**< Tensao da bateria em Volts */
    bool ppp_connected;      /**< Status da conexao celular PPP */
    bool mqtt_connected;     /**< Status da conexao MQTT */
    bool mesh_active;        /**< Status da rede BLE Mesh */
    bool gps_fix;            /**< Status do fix GPS */
    double gps_latitude;     /**< Latitude GPS (se disponivel) */
    double gps_longitude;    /**< Longitude GPS (se disponivel) */
    uint32_t tags_read_count;/**< Contador total de tags lidas na sessao */
} bastao_device_status_t;

/* --- Variaveis Globais --- */

/**
 * @brief Configuracao atual do bastao (acessivel por outros modulos).
 */
extern bastao_config_t bastao_current_config;

/**
 * @brief Status atualizado do dispositivo (preenchido pelo main loop).
 */
extern bastao_device_status_t bastao_current_status;

/**
 * @brief Buffer da ultima tag lida para notificacao via BLE.
 */
extern char ble_last_tag_json[BLE_MOBILE_ATTR_MAX_LEN];

/* --- Funcoes de Interface Publica --- */

/**
 * @brief Inicializa o servidor GATT BLE para comunicacao com app movel.
 * @details Registra o servico GATT, suas caracteristicas, configura o GAP
 *          com pareamento seguro (autenticacao + criptografia) e inicia
 *          o advertising para descoberta pelo app.
 *
 * @param[in] device_name Nome do dispositivo BLE visivel no scan (ex: "Bastao-001").
 *
 * @pre O controlador Bluetooth e o Bluedroid devem estar inicializados
 *      (feito pelo mesh_coordinator_init).
 * @post O ESP32 esta anunciando via BLE e aceitando conexoes do app.
 *
 * @return esp_err_t ESP_OK se o servidor GATT foi registrado com sucesso.
 */
esp_err_t ble_mobile_init(const char *device_name);

/**
 * @brief Atualiza a caracteristica de ultima tag lida e notifica o app.
 * @details Chamada pelo dispatcher quando uma nova tag e lida, para que
 *          o app conectado receba a notificacao em tempo real.
 *
 * @param[in] tag_json String JSON da tag (ex: {"model":"YRM100","tag":"ABCD..."}).
 *
 * @return esp_err_t ESP_OK se a notificacao foi enviada.
 */
esp_err_t ble_mobile_notify_tag(const char *tag_json);

/**
 * @brief Atualiza a caracteristica de status do dispositivo.
 * @details Deve ser chamada periodicamente pelo main loop para manter
 *          o app informado sobre o estado do bastao.
 *
 * @param[in] status Ponteiro para a estrutura de status atualizada.
 *
 * @return esp_err_t ESP_OK se o status foi atualizado.
 */
esp_err_t ble_mobile_update_status(const bastao_device_status_t *status);

/**
 * @brief Carrega a configuracao salva no NVS Flash para a RAM.
 * @details Recupera os parametros de configuracao previamente gravados
 *          pelo app e preenche a estrutura bastao_current_config.
 *
 * @return esp_err_t ESP_OK se a configuracao foi carregada.
 *                   ESP_ERR_NOT_FOUND se nao houver config salva (usa defaults).
 */
esp_err_t ble_mobile_load_config_from_nvs(void);

/**
 * @brief Verifica se ha um dispositivo movel conectado via BLE.
 *
 * @return true se um app esta conectado ao GATT Server.
 */
bool ble_mobile_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* BLE_MOBILE_H */
