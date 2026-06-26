/**
 * @file mesh_coordinator.c
 * @brief Coordenador BLE Mesh para v5.4/v5.5 - Provisioner para K10
 *        Gerencia envio de dados RFID para a tela e recepcao de
 *        status (bateria, acelerometro, eventos UI) da K10.
 *
 * @details === ATENCAO: CID do Vendor Model ===
 *          NUNCA use CID=0xFFFF para vendor models no ESP-IDF v5.4.
 *          Internamente, o stack BLE Mesh define CID_NVAL = 0xFFFF
 *          como flag para indicar modelo SIG. Se CID=0xFFFF,
 *          a funcao bt_mesh_provisioner_bind_local_model_app_idx()
 *          procura um modelo SIG (mod_id) em vez do vendor model
 *          (mod_id + cid) — o bind falha silenciosamente.
 *
 *          Solucao: usar CID registrado na Bluetooth SIG.
 *          0x02A5 = Espressif (garantido funcionar).
 *
 * @see https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
 */
#include "mesh_coordinator.h"
#include "ble_mobile.h"
#include <string.h>
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if defined(CONFIG_BLE_MESH)
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#endif

static const char *TAG = "MESH_COORDINATOR";
static bool mesh_initialized = false;

#if defined(CONFIG_BLE_MESH)

#define CID 0x02A5  /* ATENCAO: 0x02A5 = Espressif. NUNCA use 0xFFFF (CID_NVAL)! */
#define MID 0x0001
#define OP_RFID           0xC00001  /* Coordenador → K10 (dados RFID) */
#define OP_ACCEL          0xC00002  /* K10 → Coordenador (acelerometro) */
#define OP_DISPLAY_STATUS 0xC00003  /* K10 → Coordenador (bateria/eventos UI) */

static uint16_t net_key_idx = 0;
static uint16_t app_key_idx = 0;
static uint8_t  dev_uuid[16];
static uint16_t k10_addr = 0x0001;
static bool     k10_provisioned = false;
static bool     local_model_bound = false;

static const uint8_t mesh_app_key[16] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00
};

static void handle_vendor_message(uint32_t opcode, esp_ble_mesh_msg_ctx_t *ctx, uint16_t length, uint8_t *msg)
{
    if (opcode == OP_ACCEL && length > 0 && msg) {
        char json[256] = {0};
        int copy = length < 255 ? length : 255;
        memcpy(json, msg, copy);
        float ax=0, ay=0, az=0; int mov=0;
        if (sscanf(json, "{\"type\":\"accel\",\"x\":%f,\"y\":%f,\"z\":%f,\"movement\":%d", &ax, &ay, &az, &mov) >= 4) {
            bastao_current_status.accel_x = ax;
            bastao_current_status.accel_y = ay;
            bastao_current_status.accel_z = az;
            bastao_current_status.movement_detected = mov;
            bastao_current_status.k10_battery_valid = 0; /* Marca para nova leitura */
            ESP_LOGI(TAG, "Acel da K10: x=%.2f y=%.2f z=%.2f mov=%d", ax, ay, az, mov);
        }
    }
    else if (opcode == OP_DISPLAY_STATUS && length > 0 && msg) {
        char json[256] = {0};
        int copy = length < 255 ? length : 255;
        memcpy(json, msg, copy);
        float volt = 0; unsigned pct = 0, crit = 0, screen = 0;
        char event[32] = {0};
        if (sscanf(json, "{\"type\":\"display\",\"volt\":%f,\"pct\":%u,\"crit\":%u,\"screen\":%u",
                   &volt, &pct, &crit, &screen) >= 4) {
            bastao_current_status.k10_battery_voltage = volt;
            bastao_current_status.k10_battery_percentage = (uint8_t)pct;
            bastao_current_status.k10_battery_critical = (uint8_t)crit;
            bastao_current_status.k10_screen_active = (uint8_t)screen;
            bastao_current_status.k10_battery_valid = 1;
            bastao_current_status.battery_voltage = volt; /* Sincroniza com o status geral */
            ESP_LOGI(TAG, "Display K10: bat=%.2fV %u%% crit=%u screen=%u",
                     volt, pct, crit, screen);
            /* Verifica se tem evento UI */
            char *ev = strstr(json, "\"event\":\"");
            if (ev) {
                ev += 9;
                char *end = strchr(ev, '"');
                if (end) {
                    int len = (int)(end - ev);
                    if (len > 0 && len < 31) {
                        memcpy(event, ev, len);
                        event[len] = '\0';
                        ESP_LOGI(TAG, "Evento UI K10: %s", event);
                    }
                }
            }
        }
    }
}

static esp_ble_mesh_model_op_t vnd_op[] = {
    ESP_BLE_MESH_MODEL_OP(OP_RFID, 0),
    ESP_BLE_MESH_MODEL_OP(OP_ACCEL, 0),
    ESP_BLE_MESH_MODEL_OP(OP_DISPLAY_STATUS, 0),
    ESP_BLE_MESH_MODEL_OP_END,
};

static esp_ble_mesh_cfg_srv_t cfg_srv;
static esp_ble_mesh_client_t config_client;
static esp_ble_mesh_client_t vendor_client;

static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&cfg_srv),
    ESP_BLE_MESH_MODEL_CFG_CLI(&config_client),
};

static esp_ble_mesh_model_t vnd_models[] = {
    ESP_BLE_MESH_VENDOR_MODEL(CID, MID, vnd_op, NULL, &vendor_client),
};

static esp_ble_mesh_elem_t elems[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, vnd_models),
};

static esp_ble_mesh_comp_t composition = {
    .cid = CID, .elements = elems, .element_count = 1,
};

static void config_client_cb(esp_ble_mesh_cfg_client_cb_event_t event,
                             esp_ble_mesh_cfg_client_cb_param_t *param)
{
    uint32_t opcode = param->params->opcode;
    uint16_t addr = param->params->ctx.addr;

    ESP_LOGI(TAG, "config_client_cb: event=%d opcode=0x%04" PRIx32 " addr=0x%04x err=%d",
             event, opcode, addr, param->error_code);

    if (param->error_code) {
        ESP_LOGE(TAG, "Config client error: event=0x%02x opcode=0x%04" PRIx32 " addr=0x%04x err=0x%02x",
                 event, opcode, addr, param->error_code);
        /* Retry: se APP_KEY_ADD falhou, tenta novamente apos 2s */
        if (opcode == ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD && !k10_provisioned) {
            ESP_LOGW(TAG, "Tentando reenviar AppKey para no 0x%04x em 2s...", addr);
            vTaskDelay(pdMS_TO_TICKS(2000));
            esp_ble_mesh_client_common_param_t retry_common = {0};
            retry_common.opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD;
            retry_common.model = config_client.model;
            retry_common.ctx.net_idx = net_key_idx;
            retry_common.ctx.app_idx = app_key_idx;
            retry_common.ctx.addr = addr;
            retry_common.ctx.send_ttl = 4;
            retry_common.msg_timeout = 0;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
            retry_common.msg_role = ROLE_PROVISIONER;
#endif
            esp_ble_mesh_cfg_client_set_state_t retry_state = {0};
            retry_state.app_key_add.net_idx = net_key_idx;
            retry_state.app_key_add.app_idx = app_key_idx;
            memcpy(retry_state.app_key_add.app_key, mesh_app_key, 16);
            esp_ble_mesh_config_client_set_state(&retry_common, &retry_state);
        }
        return;
    }

    if (event == ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT) {
        if (opcode == ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD) {
            ESP_LOGI(TAG, "AppKey adicionado no no 0x%04x com sucesso. Vinculando ao modelo vendor...", addr);
            
            esp_ble_mesh_client_common_param_t common = {0};
            common.opcode = ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND;
            common.model = config_client.model;
            common.ctx.net_idx = net_key_idx;
            common.ctx.app_idx = app_key_idx;
            common.ctx.addr = addr;
            common.ctx.send_ttl = 4;
            common.msg_timeout = 0;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
            common.msg_role = ROLE_PROVISIONER;
#endif

            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            set_state.model_app_bind.element_addr = addr;
            set_state.model_app_bind.model_app_idx = app_key_idx;
            set_state.model_app_bind.model_id = MID;
            set_state.model_app_bind.company_id = CID;

            esp_err_t err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Falha ao enviar Model App Bind para o no 0x%04x (err %d)", addr, err);
            }
        } else if (opcode == ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND) {
            k10_provisioned = true;
            ESP_LOGI(TAG, "AppKey vinculado com sucesso no no 0x%04x! Comunicacao Mesh pronta.", addr);
        }
    }
}

static void custom_model_cb(esp_ble_mesh_model_cb_event_t event,
                            esp_ble_mesh_model_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        handle_vendor_message(param->model_operation.opcode,
                              param->model_operation.ctx,
                              param->model_operation.length,
                              param->model_operation.msg);
        break;
    case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
        if (param->model_send_comp.err_code) {
            ESP_LOGW(TAG, "Envio falhou: opcode=0x%06" PRIx32 " err=%d",
                     param->model_send_comp.opcode, param->model_send_comp.err_code);
        }
        break;
    default:
        break;
    }
}

/* --- Callback do provisionador --- */
static void prov_cb(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT:
        ESP_LOGI(TAG, "K10 detectado! Provisionando...");
        memcpy(dev_uuid, param->provisioner_recv_unprov_adv_pkt.dev_uuid, 16);
        
        esp_ble_mesh_unprov_dev_add_t add_dev = {0};
        memcpy(add_dev.addr, param->provisioner_recv_unprov_adv_pkt.addr, 6);
        add_dev.addr_type = param->provisioner_recv_unprov_adv_pkt.addr_type;
        memcpy(add_dev.uuid, dev_uuid, 16);
        add_dev.oob_info = param->provisioner_recv_unprov_adv_pkt.oob_info;
        add_dev.bearer = param->provisioner_recv_unprov_adv_pkt.bearer;

        esp_err_t err = esp_ble_mesh_provisioner_add_unprov_dev(&add_dev,
            ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_START_PROV_NOW_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Falha ao adicionar dispositivo para provisionamento (err %d)", err);
        }
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT:
        k10_addr = param->provisioner_prov_complete.unicast_addr;
        ESP_LOGI(TAG, "K10 provisionado! Endereco: 0x%04X.元素数=%d. Enviando AppKey...",
                 k10_addr, param->provisioner_prov_complete.element_num);
        
        esp_ble_mesh_client_common_param_t common = {0};
        common.opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD;
        common.model = config_client.model;
        common.ctx.net_idx = net_key_idx;
        common.ctx.app_idx = app_key_idx;
        common.ctx.addr = k10_addr;
        common.ctx.send_ttl = 4;
        common.msg_timeout = 0;
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 2, 0)
        common.msg_role = ROLE_PROVISIONER;
#endif

        esp_ble_mesh_cfg_client_set_state_t set_state = {0};
        set_state.app_key_add.net_idx = net_key_idx;
        set_state.app_key_add.app_idx = app_key_idx;
        memcpy(set_state.app_key_add.app_key, mesh_app_key, 16);

        err = esp_ble_mesh_config_client_set_state(&common, &set_state);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Falha ao enviar AppKey para o no 0x%04x (err %d)", k10_addr, err);
        }
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT:
        ESP_LOGI(TAG, "Link de provisionamento aberto.");
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT:
        ESP_LOGI(TAG, "Link de provisionamento fechado.");
        break;
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
        ESP_LOGI(TAG, "Registro de provisao concluido com status %d", param->prov_register_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT:
        ESP_LOGI(TAG, "Provisionador habilitado com status %d. Adicionando AppKey local...",
                 param->provisioner_prov_enable_comp.err_code);
        if (param->provisioner_prov_enable_comp.err_code == ESP_OK) {
            esp_err_t err = esp_ble_mesh_provisioner_add_local_app_key(mesh_app_key, net_key_idx, app_key_idx);
            ESP_LOGI(TAG, "add_local_app_key retornou: %d (net_idx=%d app_idx=%d)", err, net_key_idx, app_key_idx);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Falha ao iniciar adicao do AppKey local (err %d)", err);
            }
        }
        break;
    case ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT:
        ESP_LOGI(TAG, "ADD_LOCAL_APP_KEY_COMP: err=%d app_idx_retornado=%d",
                 param->provisioner_add_app_key_comp.err_code,
                 param->provisioner_add_app_key_comp.app_idx);
        if (param->provisioner_add_app_key_comp.err_code == ESP_OK) {
            /* USAR o app_idx retornado pelo stack, nao o valor original */
            app_key_idx = param->provisioner_add_app_key_comp.app_idx;
            uint16_t local_addr = esp_ble_mesh_get_primary_element_address();
            ESP_LOGI(TAG, "AppKey local OK (app_idx=%d). Endereco: 0x%04X. Vinculando...", app_key_idx, local_addr);

            esp_err_t err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(local_addr, app_key_idx, MID, CID);
            ESP_LOGI(TAG, "bind_api retornou: %d", err);
            if (err == ESP_OK) {
                local_model_bound = true;
                ESP_LOGI(TAG, "Vinculacao OK! Mesh pronto para envio.");

                /* Restaurar no K10 ja provisionado do banco de dados do provisioner (NVS) */
                for (uint16_t addr = 0x0005; addr < 0x0010; addr++) {
                    esp_ble_mesh_node_t *node = esp_ble_mesh_provisioner_get_node_with_addr(addr);
                    if (node != NULL) {
                        k10_addr = addr;
                        k10_provisioned = true;
                        ESP_LOGI(TAG, "Restaurado no K10 provisionado a partir da NVS do provisionador: endereco=0x%04x", k10_addr);
                        break;
                    }
                }
            } else {
                ESP_LOGE(TAG, "Falha bind: %d", err);
            }
        } else {
            ESP_LOGE(TAG, "Falha ao adicionar AppKey local: %d", param->provisioner_add_app_key_comp.err_code);
        }
        break;
    case ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT:
        ESP_LOGI(TAG, "BIND_APP_KEY_TO_MODEL_COMP: err=%d (via API publica)",
                 param->provisioner_bind_app_key_to_model_comp.err_code);
        /* Nao seta local_model_bound aqui - ja foi setado pela abordagem interna */
        break;
    default: break;
    }
}

static esp_err_t mesh_provisioner_init(void)
{
    esp_err_t err;

    esp_ble_mesh_register_prov_callback(prov_cb);
    esp_ble_mesh_register_config_client_callback(config_client_cb);
    esp_ble_mesh_register_custom_model_callback(custom_model_cb);

    static uint8_t prov_uuid[16] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01
    };

    static esp_ble_mesh_prov_t provisioner = {
        .prov_uuid          = prov_uuid,
        .prov_unicast_addr  = 0x0002,
        .prov_start_address = 0x0005,
        .prov_attention      = 0x00,
        .prov_algorithm      = 0x00,
        .prov_pub_key_oob    = 0x00,
        .prov_static_oob_val = NULL,
        .prov_static_oob_len = 0x00,
        .flags               = 0x00,
        .iv_index            = 0x00,
    };

    err = esp_ble_mesh_init(&provisioner, &composition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ble_mesh_init failed: %d", err);
        return err;
    }

    /* Inicializa o vendor model como client model (obrigatorio para envio) */
    err = esp_ble_mesh_client_model_init(&vnd_models[0]);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ble_mesh_client_model_init falhou: %d", err);
        return err;
    }
    ESP_LOGI(TAG, "Vendor client model inicializado.");

    err = esp_ble_mesh_provisioner_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ble_mesh_provisioner_prov_enable failed: %d", err);
        return err;
    }
    return ESP_OK;
}

#endif /* CONFIG_BLE_MESH */

esp_err_t mesh_coordinator_init(void)
{
    ESP_LOGI(TAG, "Inicializando Bluetooth...");
    esp_err_t err = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;
    esp_bt_controller_config_t bt = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt); if (err) return err;
    err = esp_bt_controller_enable(ESP_BT_MODE_BLE); if (err) return err;
    err = esp_bluedroid_init(); if (err) return err;
    err = esp_bluedroid_enable(); if (err) return err;
    ESP_LOGI(TAG, "Bluetooth ativo. Inicializando BLE Mesh...");
#if defined(CONFIG_BLE_MESH)
    err = mesh_provisioner_init();
    if (err) {
        ESP_LOGE(TAG, "Falha no provisioner: %d", err);
        bastao_current_status.mesh_active = false;
        return err;
    }
    ESP_LOGI(TAG, "Provisioner BLE Mesh ativo. Aguardando K10...");
    bastao_current_status.mesh_active = true;
#else
    ESP_LOGW(TAG, "CONFIG_BLE_MESH desabilitado. Modo simulado.");
    bastao_current_status.mesh_active = false;
#endif
    mesh_initialized = true;
    return ESP_OK;
}

esp_err_t mesh_coordinator_send_data(const char *hex_payload)
{
    if (!mesh_initialized || !hex_payload || !hex_payload[0]) return ESP_ERR_INVALID_STATE;
#if defined(CONFIG_BLE_MESH)
    if (!k10_provisioned || !local_model_bound) {
        ESP_LOGD(TAG, "Mesh nao pronto (prov=%d bind=%d). Payload perdido.", k10_provisioned, local_model_bound);
        return ESP_FAIL;
    }
    esp_ble_mesh_msg_ctx_t ctx = { .net_idx = net_key_idx, .app_idx = app_key_idx, .addr = k10_addr, .send_ttl = 4 };
    esp_err_t ret = esp_ble_mesh_client_model_send_msg(&vnd_models[0], &ctx, OP_RFID,
                                                        strlen(hex_payload), (uint8_t*)hex_payload,
                                                        0, false, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Send falhou: ret=%d addr=0x%04x app_idx=%d prov=%d bind=%d",
                 ret, k10_addr, app_key_idx, k10_provisioned, local_model_bound);
    }
    return ret;
#else
    ESP_LOGI(TAG, "[MESH_TX](sim) %s", hex_payload);
    return ESP_OK;
#endif
}
