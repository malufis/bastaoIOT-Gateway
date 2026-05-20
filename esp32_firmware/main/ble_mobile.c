/**
 * @file ble_mobile.c
 * @brief Implementacao do servidor GATT BLE para comunicacao com app movel.
 * @details Este modulo gerencia o ciclo de vida do GATT Server, incluindo
 *          registro de servico/caracteristicas, pareamento seguro, handlers
 *          de leitura/escrita e persistencia de dados no NVS Flash.
 *
 * @author Antigravity Agent
 * @date 2026-05-20
 */

#include "ble_mobile.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "wifi_driver.h"
#include "simcom_ppp.h"
#include "mqtt_publisher.h"

#include "esp_bt.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_common_api.h"

static const char *TAG = "BLE_MOBILE";

/* --- Definicao de Variaveis Globais --- */

bastao_config_t bastao_current_config = {
    .yrm100_power = 20,
    .scan_time_ms = 200,
    .battery_report_interval_s = 5,
    .wl134_enabled = true,
    .yrm100_enabled = true,
};

bastao_device_status_t bastao_current_status = {0};

char ble_last_tag_json[BLE_MOBILE_ATTR_MAX_LEN] = {0};

network_config_t bastao_network_config = {
    .wifi.ssid = "bastaoIOT",
    .wifi.password = "3spB@st@0",
    .wifi.enabled = true,
    .cellular.apn = "zap.vivo.com.br",
    .cellular.user = "vivo",
    .cellular.password = "vivo",
    .cellular.enabled = true,
    .mqtt.broker_uri = "mqtt://broker.example.com:1883",
    .mqtt.client_id = "bastao-esp-001",
    .mqtt.topic_telemetry = "bastao/telemetria",
    .mqtt.topic_gps = "bastao/gps",
    .mode = NETWORK_MODE_AUTO,
};

/* --- Variaveis Estaticas --- */

/** @brief ID do perfil de aplicacao GATTS. */
#define BASTAO_APP_ID 0

/** @brief Numero de handles GATT (servico + 7 chars * 2 + 2 CCCDs). */
#define GATTS_NUM_HANDLE 16

/** @brief Flag indicando se um app movel esta conectado. */
static bool mobile_connected = false;

/** @brief ID da conexao ativa. */
static uint16_t mobile_conn_id = 0;

/** @brief Interface GATTS registrada. */
static esp_gatt_if_t gatts_if_global = ESP_GATT_IF_NONE;

/** @brief Tabela de handles dos atributos GATT. */
static uint16_t handle_table[GATTS_NUM_HANDLE];

/** @brief Nome do dispositivo BLE armazenado localmente. */
static char device_name[BLE_MOBILE_DEVICE_NAME_LEN] = "Bastao-ESP";

/* --- Indices dos handles na tabela --- */
enum {
    IDX_SVC,             /**< Handle do servico */
    IDX_CHAR_CONFIG_W,   /**< Handle declaracao config write */
    IDX_CHAR_CONFIG_W_VAL, /**< Handle valor config write */
    IDX_CHAR_BUSINESS_W, /**< Handle declaracao business write */
    IDX_CHAR_BUSINESS_W_VAL, /**< Handle valor business write */
    IDX_CHAR_LAST_TAG,   /**< Handle declaracao last tag */
    IDX_CHAR_LAST_TAG_VAL, /**< Handle valor last tag */
    IDX_CHAR_LAST_TAG_CCC, /**< Handle CCCD (notify enable) */
    IDX_CHAR_STATUS,     /**< Handle declaracao status */
    IDX_CHAR_STATUS_VAL, /**< Handle valor status */
    IDX_CHAR_CELLULAR,   /**< Handle declaracao celular status */
    IDX_CHAR_CELLULAR_VAL, /**< Handle valor celular status */
    IDX_CHAR_CELLULAR_CCC, /**< Handle CCCD celular */
    IDX_CHAR_LOG,        /**< Handle declaracao log */
    IDX_CHAR_LOG_VAL,    /**< Handle valor log */
    IDX_NB,              /**< Total de handles */
};

/* --- UUIDs --- */

static const uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t char_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t char_ccc_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;

static const uint16_t bastao_svc_uuid = BASTAO_SERVICE_UUID;
static const uint16_t config_write_uuid = CHAR_CONFIG_WRITE_UUID;
static const uint16_t business_write_uuid = CHAR_BUSINESS_WRITE_UUID;
static const uint16_t last_tag_uuid = CHAR_LAST_TAG_UUID;
static const uint16_t device_status_uuid = CHAR_DEVICE_STATUS_UUID;
static const uint16_t cellular_status_uuid = CHAR_CELLULAR_STATUS_UUID;
static const uint16_t logger_uuid = CHAR_LOGGER_UUID;

static const uint8_t char_prop_write =
    ESP_GATT_CHAR_PROP_BIT_WRITE;
static const uint8_t char_prop_read =
    ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_read_notify =
    ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;

/** @brief Valor inicial do CCCD (notificacoes desabilitadas). */
static uint8_t ccc_value[2] = {0x00, 0x00};

/* --- Tabela de Atributos GATT --- */

static const esp_gatts_attr_db_t bastao_gatt_db[IDX_NB] = {
    /* Servico Principal */
    [IDX_SVC] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
         sizeof(uint16_t), sizeof(bastao_svc_uuid), (uint8_t *)&bastao_svc_uuid}
    },

    /* Caracteristica: Config Write (App -> Bastao) */
    [IDX_CHAR_CONFIG_W] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_write}
    },
    [IDX_CHAR_CONFIG_W_VAL] = {
        {ESP_GATT_RSP_BY_APP},
        {ESP_UUID_LEN_16, (uint8_t *)&config_write_uuid,
         ESP_GATT_PERM_WRITE_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },

    /* Caracteristica: Business Data Write (App -> Bastao) */
    [IDX_CHAR_BUSINESS_W] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_write}
    },
    [IDX_CHAR_BUSINESS_W_VAL] = {
        {ESP_GATT_RSP_BY_APP},
        {ESP_UUID_LEN_16, (uint8_t *)&business_write_uuid,
         ESP_GATT_PERM_WRITE_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },

    /* Caracteristica: Last Tag (Read + Notify -> App) */
    [IDX_CHAR_LAST_TAG] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_read_notify}
    },
    [IDX_CHAR_LAST_TAG_VAL] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&last_tag_uuid,
         ESP_GATT_PERM_READ_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },
    [IDX_CHAR_LAST_TAG_CCC] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_ccc_uuid,
         ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
         sizeof(uint16_t), sizeof(ccc_value), (uint8_t *)ccc_value}
    },

    /* Caracteristica: Device Status (Read -> App) */
    [IDX_CHAR_STATUS] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_read}
    },
    [IDX_CHAR_STATUS_VAL] = {
        {ESP_GATT_RSP_BY_APP},
        {ESP_UUID_LEN_16, (uint8_t *)&device_status_uuid,
         ESP_GATT_PERM_READ_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },

    /* Caracteristica: Cellular Status (Read + Notify -> App) */
    [IDX_CHAR_CELLULAR] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_read_notify}
    },
    [IDX_CHAR_CELLULAR_VAL] = {
        {ESP_GATT_RSP_BY_APP},
        {ESP_UUID_LEN_16, (uint8_t *)&cellular_status_uuid,
         ESP_GATT_PERM_READ_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },
    [IDX_CHAR_CELLULAR_CCC] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_ccc_uuid,
         ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
         sizeof(uint16_t), sizeof(ccc_value), (uint8_t *)ccc_value}
    },

    /* Caracteristica: Logger (Write -> App para receber logs) */
    [IDX_CHAR_LOG] = {
        {ESP_GATT_AUTO_RSP},
        {ESP_UUID_LEN_16, (uint8_t *)&char_declaration_uuid, ESP_GATT_PERM_READ,
         sizeof(uint8_t), sizeof(uint8_t), (uint8_t *)&char_prop_read_notify}
    },
    [IDX_CHAR_LOG_VAL] = {
        {ESP_GATT_RSP_BY_APP},
        {ESP_UUID_LEN_16, (uint8_t *)&logger_uuid,
         ESP_GATT_PERM_WRITE_ENC_MITM | ESP_GATT_PERM_READ_ENC_MITM,
         BLE_MOBILE_ATTR_MAX_LEN, 0, NULL}
    },
};

/* --- Funcoes Internas Privadas --- */

/**
 * @brief Salva os dados de configuracao recebidos do app no NVS Flash.
 *
 * @param[in] json_str String JSON contendo os parametros de configuracao.
 */
static void save_config_to_nvs(const char *json_str) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE_CONFIG, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao abrir NVS para config: %d", err);
        return;
    }

    err = nvs_set_str(handle, "cfg_json", json_str);
    if (err == ESP_OK) {
        nvs_commit(handle);
        ESP_LOGI(TAG, "Configuracao salva no NVS com sucesso.");
    } else {
        ESP_LOGE(TAG, "Falha ao gravar config no NVS: %d", err);
    }
    nvs_close(handle);
}

/**
 * @brief Salva dados de negocio (Fazenda/Lote/Animal) no NVS Flash.
 *
 * @param[in] json_str String JSON contendo os dados de negocio.
 */
static void save_business_data_to_nvs(const char *json_str) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE_BUSINESS, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao abrir NVS para dados de negocio: %d", err);
        return;
    }

    err = nvs_set_str(handle, "biz_json", json_str);
    if (err == ESP_OK) {
        nvs_commit(handle);
        ESP_LOGI(TAG, "Dados de negocio salvos no NVS.");
    } else {
        ESP_LOGE(TAG, "Falha ao gravar dados de negocio no NVS: %d", err);
    }
    nvs_close(handle);
}

/**
 * @brief Formata o status do dispositivo como JSON para leitura pelo app.
 *
 * @param[out] buf Buffer de saida para a string JSON.
 * @param[in] buf_size Tamanho do buffer.
 */
static void format_device_status_json(char *buf, size_t buf_size) {
    snprintf(buf, buf_size,
             "{\"batt\":%.2f,\"ppp\":%s,\"mqtt\":%s,\"mesh\":%s,"
             "\"gps\":%s,\"lat\":%.6f,\"lon\":%.6f,\"tags\":%lu}",
             bastao_current_status.battery_voltage,
             bastao_current_status.ppp_connected ? "true" : "false",
             bastao_current_status.mqtt_connected ? "true" : "false",
             bastao_current_status.mesh_active ? "true" : "false",
             bastao_current_status.gps_fix ? "true" : "false",
             bastao_current_status.gps_latitude,
             bastao_current_status.gps_longitude,
             (unsigned long)bastao_current_status.tags_read_count);
}

/**
 * @brief Formata o status da rede celular como JSON para leitura pelo app.
 *
 * @param[out] buf Buffer de saida para a string JSON.
 * @param[in] buf_size Tamanho do buffer.
 */
static void format_cellular_status_json(char *buf, size_t buf_size) {
    cellular_status_t cell_status;
    memset(&cell_status, 0, sizeof(cell_status));

    if (simcom_ppp_get_status(&cell_status) == ESP_OK) {
        const char *tech_str = "UNKNOWN";
        switch (cell_status.tech) {
            case CELLULAR_TECH_2G: tech_str = "2G"; break;
            case CELLULAR_TECH_3G: tech_str = "3G"; break;
            case CELLULAR_TECH_4G_LTE: tech_str = "4G LTE"; break;
            case CELLULAR_TECH_NB_IOT: tech_str = "NB-IoT"; break;
            case CELLULAR_TECH_NONE: tech_str = "NONE"; break;
            default: break;
        }

        snprintf(buf, buf_size,
                 "{\"rssi_dbm\":%d,\"ber\":%d,\"tech\":\"%s\","
                 "\"mcc\":%d,\"mnc\":%d,\"op\":\"%s\","
                 "\"reg\":%s,\"roam\":%s,\"ppp\":%s}",
                 cell_status.rssi, cell_status.ber, tech_str,
                 cell_status.mcc, cell_status.mnc, cell_status.operator_name,
                 cell_status.registered ? "true" : "false",
                 cell_status.roaming ? "true" : "false",
                 cell_status.modem_state == SIMCOM_STATE_PPP_ACTIVE ? "true" : "false");
    } else {
        snprintf(buf, buf_size, "{\"error\":\"modem_unavailable\"}");
    }
}

/**
 * @brief Handler de eventos do perfil GATTS (leitura, escrita, conexao).
 */
static void gatts_profile_event_handler(esp_gatts_cb_event_t event,
                                         esp_gatt_if_t gatts_if,
                                         esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(TAG, "GATTS registrado. App ID: %d, Status: %d",
                 param->reg.app_id, param->reg.status);
        gatts_if_global = gatts_if;

        // Define o nome do dispositivo BLE
        esp_ble_gap_set_device_name(device_name);

        // Registra a tabela de atributos GATT
        esp_ble_gatts_create_attr_tab(bastao_gatt_db, gatts_if, IDX_NB, 0);
        break;

    case ESP_GATTS_CREAT_ATTR_TAB_EVT:
        if (param->add_attr_tab.status != ESP_GATT_OK) {
            ESP_LOGE(TAG, "Falha ao criar tabela GATT: %d",
                     param->add_attr_tab.status);
            break;
        }
        if (param->add_attr_tab.num_handle != IDX_NB) {
            ESP_LOGE(TAG, "Numero de handles incorreto: %d vs %d",
                     param->add_attr_tab.num_handle, IDX_NB);
            break;
        }

        memcpy(handle_table, param->add_attr_tab.handles,
               sizeof(handle_table));
        ESP_LOGI(TAG, "Tabela GATT criada com %d handles.", IDX_NB);

        // Inicia o servico GATT
        esp_ble_gatts_start_service(handle_table[IDX_SVC]);
        break;

    case ESP_GATTS_START_EVT:
        ESP_LOGI(TAG, "Servico GATT iniciado. Iniciando advertising...");

        // Configura e inicia o advertising BLE
        esp_ble_adv_params_t adv_params = {
            .adv_int_min = 0x20,   // 20ms
            .adv_int_max = 0x40,   // 40ms
            .adv_type = ADV_TYPE_IND,
            .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
            .channel_map = ADV_CHNL_ALL,
            .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        };
        esp_ble_gap_start_advertising(&adv_params);
        break;

    case ESP_GATTS_CONNECT_EVT:
        ESP_LOGI(TAG, "App movel conectado! Conn ID: %d", param->connect.conn_id);
        mobile_connected = true;
        mobile_conn_id = param->connect.conn_id;

        // Solicita pareamento seguro
        esp_ble_set_encryption(param->connect.remote_bda,
                               ESP_BLE_SEC_ENCRYPT_MITM);
        break;

    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGW(TAG, "App movel desconectado. Motivo: %d",
                 param->disconnect.reason);
        mobile_connected = false;

        // Reinicia o advertising para permitir nova conexao
        esp_ble_adv_params_t adv_params_reconn = {
            .adv_int_min = 0x20,
            .adv_int_max = 0x40,
            .adv_type = ADV_TYPE_IND,
            .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
            .channel_map = ADV_CHNL_ALL,
            .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
        };
        esp_ble_gap_start_advertising(&adv_params_reconn);
        break;

    case ESP_GATTS_WRITE_EVT:
        if (!param->write.is_prep) {
            uint16_t attr_handle = param->write.handle;
            uint16_t len = param->write.len;
            const uint8_t *value = param->write.value;

            // Monta string terminada em nulo a partir dos dados recebidos
            char recv_buf[BLE_MOBILE_ATTR_MAX_LEN];
            size_t copy_len = (len < sizeof(recv_buf) - 1) ? len : sizeof(recv_buf) - 1;
            memcpy(recv_buf, value, copy_len);
            recv_buf[copy_len] = '\0';

            // Identifica qual caracteristica foi escrita
            if (attr_handle == handle_table[IDX_CHAR_CONFIG_W_VAL]) {
                ESP_LOGI(TAG, "[BLE_RX] Config recebida (%d bytes): %s",
                         len, recv_buf);
                save_config_to_nvs(recv_buf);

                // TODO: Parsear JSON e atualizar bastao_current_config em RAM
            }
            else if (attr_handle == handle_table[IDX_CHAR_BUSINESS_W_VAL]) {
                ESP_LOGI(TAG, "[BLE_RX] Dados de negocio recebidos (%d bytes): %s",
                         len, recv_buf);
                save_business_data_to_nvs(recv_buf);
            }

            // Envia resposta de confirmacao (se necessario)
            if (param->write.need_rsp) {
                esp_ble_gatts_send_response(gatts_if, param->write.conn_id,
                                            param->write.trans_id,
                                            ESP_GATT_OK, NULL);
            }
        }
        break;

    case ESP_GATTS_READ_EVT: {
        uint16_t attr_handle = param->read.handle;

        if (attr_handle == handle_table[IDX_CHAR_STATUS_VAL]) {
            // Monta o JSON de status em tempo real
            char status_json[BLE_MOBILE_ATTR_MAX_LEN];
            format_device_status_json(status_json, sizeof(status_json));

            esp_gatt_rsp_t rsp;
            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = attr_handle;
            rsp.attr_value.len = strlen(status_json);
            memcpy(rsp.attr_value.value, status_json, rsp.attr_value.len);

            esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
                                        param->read.trans_id,
                                        ESP_GATT_OK, &rsp);

            ESP_LOGD(TAG, "[BLE_TX] Status enviado: %s", status_json);
        } else if (attr_handle == handle_table[IDX_CHAR_CELLULAR_VAL]) {
            char cell_json[BLE_MOBILE_ATTR_MAX_LEN];
            format_cellular_status_json(cell_json, sizeof(cell_json));

            esp_gatt_rsp_t rsp;
            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
            rsp.attr_value.handle = attr_handle;
            rsp.attr_value.len = strlen(cell_json);
            memcpy(rsp.attr_value.value, cell_json, rsp.attr_value.len);

            esp_ble_gatts_send_response(gatts_if, param->read.conn_id,
                                        param->read.trans_id,
                                        ESP_GATT_OK, &rsp);

            ESP_LOGD(TAG, "[BLE_TX] Cellular Status: %s", cell_json);
        }
        break;
    }

    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(TAG, "MTU negociado: %d bytes", param->mtu.mtu);
        break;

    default:
        break;
    }
}

/**
 * @brief Handler de eventos GAP (pareamento, autenticacao, chave).
 */
static void gap_event_handler(esp_gap_ble_cb_event_t event,
                               esp_ble_gap_cb_param_t *param) {
    switch (event) {
    case ESP_GAP_BLE_SEC_REQ_EVT:
        ESP_LOGI(TAG, "Requisicao de pareamento seguro recebida.");
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;

    case ESP_GAP_BLE_AUTH_CMPL_EVT:
        if (param->ble_security.auth_cmpl.success) {
            ESP_LOGI(TAG, "Pareamento concluido com sucesso. Modo: %d",
                     param->ble_security.auth_cmpl.auth_mode);
        } else {
            ESP_LOGE(TAG, "Falha no pareamento. Motivo: 0x%x",
                     param->ble_security.auth_cmpl.fail_reason);
        }
        break;

    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(TAG, "Advertising BLE iniciado com sucesso.");
        } else {
            ESP_LOGE(TAG, "Falha ao iniciar advertising: %d",
                     param->adv_start_cmpl.status);
        }
        break;

    default:
        break;
    }
}

/* --- Implementacao de Funcoes Publicas --- */

esp_err_t ble_mobile_init(const char *dev_name) {
    if (dev_name != NULL) {
        strncpy(device_name, dev_name, sizeof(device_name) - 1);
    }

    ESP_LOGI(TAG, "Inicializando GATT Server BLE Mobile: '%s'", device_name);

    // 1. Registra o handler de eventos GAP (seguranca/pareamento)
    esp_err_t err = esp_ble_gap_register_callback(gap_event_handler);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao registrar callback GAP: %d", err);
        return err;
    }

    // 2. Registra o handler de eventos GATTS
    err = esp_ble_gatts_register_callback(gatts_profile_event_handler);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao registrar callback GATTS: %d", err);
        return err;
    }

    // 3. Registra o perfil de aplicacao GATTS
    err = esp_ble_gatts_app_register(BASTAO_APP_ID);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao registrar app GATTS: %d", err);
        return err;
    }

    // 4. Configura parametros de seguranca BLE (pareamento com MITM)
    esp_ble_auth_req_t auth_req = ESP_LE_AUTH_REQ_SC_MITM_BOND;
    esp_ble_io_cap_t iocap = ESP_IO_CAP_NONE; // Just Works (sem display/teclado)
    uint8_t key_size = 16;
    uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
    uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;

    esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE,
                                   &auth_req, sizeof(auth_req));
    esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE,
                                   &iocap, sizeof(iocap));
    esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE,
                                   &key_size, sizeof(key_size));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY,
                                   &init_key, sizeof(init_key));
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY,
                                   &rsp_key, sizeof(rsp_key));

    // 5. Define o MTU local maximo
    esp_ble_gatt_set_local_mtu(517);

    // 6. Carrega configuracao salva do NVS
    ble_mobile_load_config_from_nvs();

    ESP_LOGI(TAG, "GATT Server BLE Mobile inicializado.");
    return ESP_OK;
}

esp_err_t ble_mobile_notify_tag(const char *tag_json) {
    if (tag_json == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Atualiza o buffer global da ultima tag
    strncpy(ble_last_tag_json, tag_json, sizeof(ble_last_tag_json) - 1);

    // Atualiza o valor da caracteristica no GATT
    esp_ble_gatts_set_attr_value(handle_table[IDX_CHAR_LAST_TAG_VAL],
                                 strlen(tag_json), (uint8_t *)tag_json);

    // Envia notificacao se o app estiver conectado e com notify habilitado
    if (mobile_connected && gatts_if_global != ESP_GATT_IF_NONE) {
        esp_ble_gatts_send_indicate(gatts_if_global, mobile_conn_id,
                                    handle_table[IDX_CHAR_LAST_TAG_VAL],
                                    strlen(tag_json), (uint8_t *)tag_json,
                                    false); // false = notification (nao indication)

        ESP_LOGD(TAG, "[BLE_NOTIFY] Tag enviada ao app: %s", tag_json);
    }

    return ESP_OK;
}

esp_err_t ble_mobile_notify_cellular_status(void) {
    if (gatts_if_global == ESP_GATT_IF_NONE || !mobile_connected) {
        return ESP_OK;
    }

    char cell_json[BLE_MOBILE_ATTR_MAX_LEN];
    format_cellular_status_json(cell_json, sizeof(cell_json));

    esp_ble_gatts_set_attr_value(handle_table[IDX_CHAR_CELLULAR_VAL],
                                 strlen(cell_json), (uint8_t *)cell_json);

    esp_ble_gatts_send_indicate(gatts_if_global, mobile_conn_id,
                                handle_table[IDX_CHAR_CELLULAR_VAL],
                                strlen(cell_json), (uint8_t *)cell_json,
                                false);

    ESP_LOGI(TAG, "[BLE_NOTIFY] Cellular status atualizado: %s", cell_json);
    return ESP_OK;
}

esp_err_t ble_mobile_update_status(const bastao_device_status_t *status) {
    if (status == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    memcpy(&bastao_current_status, status, sizeof(bastao_device_status_t));
    return ESP_OK;
}

esp_err_t ble_mobile_load_config_from_nvs(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE_CONFIG, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Nenhuma config salva no NVS. Usando defaults.");
        return ESP_ERR_NOT_FOUND;
    }

    char cfg_json[BLE_MOBILE_ATTR_MAX_LEN];
    size_t required_size = sizeof(cfg_json);
    err = nvs_get_str(handle, "cfg_json", cfg_json, &required_size);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Config carregada do NVS: %s", cfg_json);
        // TODO: Parsear JSON com cJSON e preencher bastao_current_config
    } else {
        ESP_LOGW(TAG, "Chave cfg_json nao encontrada no NVS.");
    }

    nvs_close(handle);
    return err;
}

bool ble_mobile_is_connected(void) { return mobile_connected; }

esp_err_t ble_mobile_load_network_config(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE_CONFIG, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "NVS nao disponivel. Usando defaults de rede.");
        return ESP_ERR_NOT_FOUND;
    }

    size_t len;

    len = sizeof(bastao_network_config.wifi.ssid);
    nvs_get_str(handle, NVS_KEY_WIFI_SSID, bastao_network_config.wifi.ssid, &len);

    len = sizeof(bastao_network_config.wifi.password);
    nvs_get_str(handle, NVS_KEY_WIFI_PASSWORD, bastao_network_config.wifi.password, &len);

    uint8_t wifi_en = 0;
    if (nvs_get_u8(handle, NVS_KEY_WIFI_ENABLED, &wifi_en) == ESP_OK) {
        bastao_network_config.wifi.enabled = (wifi_en == 1);
    }

    len = sizeof(bastao_network_config.cellular.apn);
    nvs_get_str(handle, NVS_KEY_APN, bastao_network_config.cellular.apn, &len);

    len = sizeof(bastao_network_config.cellular.user);
    nvs_get_str(handle, NVS_KEY_APN_USER, bastao_network_config.cellular.user, &len);

    len = sizeof(bastao_network_config.cellular.password);
    nvs_get_str(handle, NVS_KEY_APN_PASSWORD, bastao_network_config.cellular.password, &len);

    uint8_t cell_en = 0;
    if (nvs_get_u8(handle, NVS_KEY_CELLULAR_ENABLED, &cell_en) == ESP_OK) {
        bastao_network_config.cellular.enabled = (cell_en == 1);
    }

    len = sizeof(bastao_network_config.mqtt.broker_uri);
    nvs_get_str(handle, NVS_KEY_MQTT_URI, bastao_network_config.mqtt.broker_uri, &len);

    len = sizeof(bastao_network_config.mqtt.client_id);
    nvs_get_str(handle, NVS_KEY_MQTT_CLIENT_ID, bastao_network_config.mqtt.client_id, &len);

    len = sizeof(bastao_network_config.mqtt.topic_telemetry);
    nvs_get_str(handle, NVS_KEY_MQTT_TOPIC_TELE, bastao_network_config.mqtt.topic_telemetry, &len);

    len = sizeof(bastao_network_config.mqtt.topic_gps);
    nvs_get_str(handle, NVS_KEY_MQTT_TOPIC_GPS, bastao_network_config.mqtt.topic_gps, &len);

    uint8_t net_mode = 0;
    if (nvs_get_u8(handle, NVS_KEY_NETWORK_MODE, &net_mode) == ESP_OK) {
        bastao_network_config.mode = (network_mode_t)net_mode;
    }

    nvs_close(handle);

    ESP_LOGI(TAG, "Config de rede carregada do NVS.");
    ESP_LOGI(TAG, "  WiFi: %s (%s)", bastao_network_config.wifi.ssid,
             bastao_network_config.wifi.enabled ? "ON" : "OFF");
    ESP_LOGI(TAG, "  Celular: %s (%s)", bastao_network_config.cellular.apn,
             bastao_network_config.cellular.enabled ? "ON" : "OFF");
    ESP_LOGI(TAG, "  MQTT: %s", bastao_network_config.mqtt.broker_uri);

    return ESP_OK;
}

esp_err_t ble_mobile_save_network_config(const network_config_t *config) {
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE_CONFIG, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao abrir NVS: %d", err);
        return err;
    }

    nvs_set_str(handle, NVS_KEY_WIFI_SSID, config->wifi.ssid);
    nvs_set_str(handle, NVS_KEY_WIFI_PASSWORD, config->wifi.password);
    nvs_set_u8(handle, NVS_KEY_WIFI_ENABLED, config->wifi.enabled ? 1 : 0);

    nvs_set_str(handle, NVS_KEY_APN, config->cellular.apn);
    nvs_set_str(handle, NVS_KEY_APN_USER, config->cellular.user);
    nvs_set_str(handle, NVS_KEY_APN_PASSWORD, config->cellular.password);
    nvs_set_u8(handle, NVS_KEY_CELLULAR_ENABLED, config->cellular.enabled ? 1 : 0);

    nvs_set_str(handle, NVS_KEY_MQTT_URI, config->mqtt.broker_uri);
    nvs_set_str(handle, NVS_KEY_MQTT_CLIENT_ID, config->mqtt.client_id);
    nvs_set_str(handle, NVS_KEY_MQTT_TOPIC_TELE, config->mqtt.topic_telemetry);
    nvs_set_str(handle, NVS_KEY_MQTT_TOPIC_GPS, config->mqtt.topic_gps);

    nvs_set_u8(handle, NVS_KEY_NETWORK_MODE, (uint8_t)config->mode);

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao salvar config NVS: %d", err);
    } else {
        ESP_LOGI(TAG, "Config de rede salva no NVS.");
    }

    nvs_close(handle);
    return err;
}

esp_err_t ble_mobile_process_network_json(const char *json) {
    if (json == NULL || strlen(json) == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Processando JSON de rede: %s", json);

    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        ESP_LOGE(TAG, "Falha ao parsear JSON de rede");
        return ESP_FAIL;
    }

    cJSON *wifi = cJSON_GetObjectItem(root, "wifi");
    if (wifi != NULL) {
        cJSON *ssid = cJSON_GetObjectItem(wifi, "ssid");
        if (ssid && cJSON_IsString(ssid)) {
            strncpy(bastao_network_config.wifi.ssid, ssid->valuestring, sizeof(bastao_network_config.wifi.ssid) - 1);
        }
        cJSON *pass = cJSON_GetObjectItem(wifi, "password");
        if (pass && cJSON_IsString(pass)) {
            strncpy(bastao_network_config.wifi.password, pass->valuestring, sizeof(bastao_network_config.wifi.password) - 1);
        }
        cJSON *enabled = cJSON_GetObjectItem(wifi, "enabled");
        if (enabled) {
            bastao_network_config.wifi.enabled = cJSON_IsTrue(enabled) || enabled->valueint == 1;
        }
    }

    cJSON *cellular = cJSON_GetObjectItem(root, "cellular");
    if (cellular != NULL) {
        cJSON *apn = cJSON_GetObjectItem(cellular, "apn");
        if (apn && cJSON_IsString(apn)) {
            strncpy(bastao_network_config.cellular.apn, apn->valuestring, sizeof(bastao_network_config.cellular.apn) - 1);
        }
        cJSON *user = cJSON_GetObjectItem(cellular, "user");
        if (user && cJSON_IsString(user)) {
            strncpy(bastao_network_config.cellular.user, user->valuestring, sizeof(bastao_network_config.cellular.user) - 1);
        }
        cJSON *pass = cJSON_GetObjectItem(cellular, "password");
        if (pass && cJSON_IsString(pass)) {
            strncpy(bastao_network_config.cellular.password, pass->valuestring, sizeof(bastao_network_config.cellular.password) - 1);
        }
        cJSON *enabled = cJSON_GetObjectItem(cellular, "enabled");
        if (enabled) {
            bastao_network_config.cellular.enabled = cJSON_IsTrue(enabled) || enabled->valueint == 1;
        }
    }

    cJSON *mqtt = cJSON_GetObjectItem(root, "mqtt");
    if (mqtt != NULL) {
        cJSON *uri = cJSON_GetObjectItem(mqtt, "broker_uri");
        if (uri && cJSON_IsString(uri)) {
            strncpy(bastao_network_config.mqtt.broker_uri, uri->valuestring, sizeof(bastao_network_config.mqtt.broker_uri) - 1);
        }
        cJSON *client = cJSON_GetObjectItem(mqtt, "client_id");
        if (client && cJSON_IsString(client)) {
            strncpy(bastao_network_config.mqtt.client_id, client->valuestring, sizeof(bastao_network_config.mqtt.client_id) - 1);
        }
        cJSON *tele = cJSON_GetObjectItem(mqtt, "topic_telemetry");
        if (tele && cJSON_IsString(tele)) {
            strncpy(bastao_network_config.mqtt.topic_telemetry, tele->valuestring, sizeof(bastao_network_config.mqtt.topic_telemetry) - 1);
        }
        cJSON *gps = cJSON_GetObjectItem(mqtt, "topic_gps");
        if (gps && cJSON_IsString(gps)) {
            strncpy(bastao_network_config.mqtt.topic_gps, gps->valuestring, sizeof(bastao_network_config.mqtt.topic_gps) - 1);
        }
    }

    cJSON *mode = cJSON_GetObjectItem(root, "network_mode");
    if (mode && cJSON_IsNumber(mode)) {
        bastao_network_config.mode = (network_mode_t)(mode->valueint % 4);
    }

    cJSON_Delete(root);

    ESP_LOGI(TAG, "JSON de rede processado. Salvando...");
    return ble_mobile_save_network_config(&bastao_network_config);
}

esp_err_t ble_mobile_apply_network_config(void) {
    ESP_LOGI(TAG, "Aplicando configuracao de rede...");

    if (bastao_network_config.wifi.enabled) {
        ESP_LOGI(TAG, "Conectando Wi-Fi: %s", bastao_network_config.wifi.ssid);
        wifi_driver_connect(bastao_network_config.wifi.ssid, bastao_network_config.wifi.password);
    }

    if (bastao_network_config.cellular.enabled) {
        ESP_LOGI(TAG, "Configurando celular: %s", bastao_network_config.cellular.apn);
        simcom_apn_config_t apn = {0};
        strncpy(apn.apn, bastao_network_config.cellular.apn, sizeof(apn.apn) - 1);
        strncpy(apn.user, bastao_network_config.cellular.user, sizeof(apn.user) - 1);
        strncpy(apn.password, bastao_network_config.cellular.password, sizeof(apn.password) - 1);
        simcom_ppp_configure_apn(&apn);
    }

    ESP_LOGI(TAG, "Config de rede aplicada com sucesso.");
    return ESP_OK;
}

static bool ble_log_enabled = false;

esp_err_t ble_mobile_notify_log(const char *log_str) {
    if (log_str == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!ble_log_enabled || !mobile_connected || gatts_if_global == ESP_GATT_IF_NONE) {
        return ESP_OK;
    }

    esp_ble_gatts_set_attr_value(handle_table[IDX_CHAR_LOG_VAL],
                                 strlen(log_str), (uint8_t *)log_str);

    esp_ble_gatts_send_indicate(gatts_if_global, mobile_conn_id,
                                handle_table[IDX_CHAR_LOG_VAL],
                                strlen(log_str), (uint8_t *)log_str,
                                false);

    return ESP_OK;
}

void ble_mobile_log_enable(bool enable) {
    ble_log_enabled = enable;
    ESP_LOGI(TAG, "BLE logging %s", enable ? "habilitado" : "desabilitado");
}

bool ble_mobile_log_is_enabled(void) {
    return ble_log_enabled;
}
