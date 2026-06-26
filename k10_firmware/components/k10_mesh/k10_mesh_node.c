/**
 * @file k10_mesh_node.c
 * @brief No BLE Mesh da K10 para v5.4/v5.5 - Receptor de dados RFID e
 *        transmissor de status da tela (bateria, eventos UI)
 *
 * @details === ATENCAO: CID do Vendor Model ===
 *          NUNCA use CID=0xFFFF para vendor models no ESP-IDF v5.4.
 *          Internamente, o stack BLE Mesh define CID_NVAL = 0xFFFF
 *          como flag para indicar modelo SIG. Se CID=0xFFFF,
 *          a funcao bt_mesh_provisioner_bind_local_model_app_idx()
 *          procura um modelo SIG em vez do vendor model — bind
 *          falso-positivo.
 *
 *          Solucao: usar CID registrado na Bluetooth SIG.
 *          0x02A5 = Espressif (garantido funcionar).
 *
 *          === Dados Recebidos ===
 *          O K10 espera JSON SIMPLES (nao encriptado) do coordenador.
 *          O payload e processado por k10_mesh_process_payload() que
 *          usa strstr() para identificar o tipo ("rfid" ou "batt").
 *          Nao ha decodificacao AES-256-CBC — a seguranca e garantida
 *          pela criptografia de link layer do BLE Mesh (AppKey + NetKey).
 */
#include "k10_mesh_node.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#if defined(CONFIG_BLE_MESH)
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#endif

static const char *TAG = "K10_MESH";
static k10_last_rfid_t last_rfid = {0};
static k10_battery_status_t battery_status = {0};
static k10_alert_t current_alert = {0};
static k10_gps_data_t gps_data = {0};
static k10_cell_status_t cell_status = {0};
static k10_gateway_status_t gateway_status = {0};
static bool mesh_ready = false;

#if defined(CONFIG_BLE_MESH)

#define CID 0x02A5  /* ATENCAO: 0x02A5 = Espressif. NUNCA use 0xFFFF (CID_NVAL)! */
#define MID 0x0001
#define OP_RFID           0xC00001  /* Coordenador → K10 (dados RFID) */
#define OP_ACCEL          0xC00002  /* K10 → Coordenador (acelerometro) */
#define OP_DISPLAY_STATUS 0xC00003  /* K10 → Coordenador (bateria/eventos UI) */

static uint16_t coord_addr = 0;
static uint8_t  dev_uuid[16] = {0};
static esp_ble_mesh_cfg_srv_t cfg_srv;

static void handle_vendor_message(uint32_t opcode, esp_ble_mesh_msg_ctx_t *ctx, uint16_t length, uint8_t *msg)
{
    if (opcode == OP_RFID && length > 0 && msg) {
        coord_addr = ctx->addr;
        mesh_ready = true;
        char *payload = malloc(length + 1);
        if (payload) {
            memcpy(payload, msg, length);
            payload[length] = '\0';
            ESP_LOGI(TAG, "Recebido do coordenador: %s", payload);
            k10_mesh_process_payload(payload);
            free(payload);
        }
    }
}

/* --- Callback de provisionamento do node --- */
static void prov_cb(esp_ble_mesh_prov_cb_event_t event,
                    esp_ble_mesh_prov_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
        ESP_LOGI(TAG, "Registro de provisao concluido: %d", param->prov_register_comp.err_code);
        break;
    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
        ESP_LOGI(TAG, "Provisionado! Endereco: 0x%04X net_idx=%d",
                 param->node_prov_complete.addr,
                 param->node_prov_complete.net_idx);
        mesh_ready = true;
        break;
    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
        ESP_LOGI(TAG, "Link de provisionamento aberto.");
        break;
    case ESP_BLE_MESH_NODE_PROV_LINK_CLOSE_EVT:
        ESP_LOGI(TAG, "Link de provisionamento fechado. reason=%d",
                 param->node_prov_link_close.reason);
        break;
    case ESP_BLE_MESH_NODE_SET_UNPROV_DEV_NAME_COMP_EVT:
        break;
    default:
        ESP_LOGW(TAG, "prov_cb evento desconhecido: %d", event);
        break;
    }
}

static void custom_model_cb(esp_ble_mesh_model_cb_event_t event,
                            esp_ble_mesh_model_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        ESP_LOGI(TAG, "MODEL_OPERATION: opcode=0x%06" PRIx32 " addr=0x%04x len=%d",
                 param->model_operation.opcode,
                 param->model_operation.ctx ? param->model_operation.ctx->addr : 0,
                 param->model_operation.length);
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
        ESP_LOGW(TAG, "custom_model_cb evento desconhecido: %d", event);
        break;
    }
}

static esp_ble_mesh_model_op_t vnd_op[] = {
    ESP_BLE_MESH_MODEL_OP(OP_RFID, 0),
    ESP_BLE_MESH_MODEL_OP(OP_ACCEL, 0),
    ESP_BLE_MESH_MODEL_OP(OP_DISPLAY_STATUS, 0),
    ESP_BLE_MESH_MODEL_OP_END,
};

static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&cfg_srv),
};

static esp_ble_mesh_model_t vnd_models[] = {
    ESP_BLE_MESH_VENDOR_MODEL(CID, MID, vnd_op, NULL, NULL),
};

static esp_ble_mesh_elem_t elems[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, vnd_models),
};

static esp_ble_mesh_comp_t composition = {
    .cid = CID, .elements = elems, .element_count = 1,
};

/* Gera UUID unico baseado no MAC do dispositivo */
static void generate_dev_uuid(uint8_t *uuid)
{
    const uint8_t *mac = esp_bt_dev_get_address();
    if (mac) {
        memcpy(uuid, mac, 6);
        uuid[6] = 0x4B; /* 'K' */
        uuid[7] = 0x31; /* '1' */
        uuid[8] = 0x30; /* '0' */
        /* Preenche resto com 0 */
        memset(uuid + 9, 0, 7);
    } else {
        /* Fallback: UUID padrao K10 */
        memcpy(uuid, "K10_BASTAO_MESH", 16);
    }
}

#endif /* CONFIG_BLE_MESH */

esp_err_t k10_mesh_node_init(void)
{
    ESP_LOGI(TAG, "Inicializando no BLE Mesh...");
    mesh_ready = false;
    esp_err_t err;

    err = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;
    esp_bt_controller_config_t bt = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt); if (err) return err;
    err = esp_bt_controller_enable(ESP_BT_MODE_BLE); if (err) return err;
    err = esp_bluedroid_init(); if (err) return err;
    err = esp_bluedroid_enable(); if (err) return err;

#if defined(CONFIG_BLE_MESH)
    /* Configuracao do Node BLE Mesh */
    generate_dev_uuid(dev_uuid);
    static esp_ble_mesh_prov_t node_config = {
        .uuid = dev_uuid,
    };
    esp_ble_mesh_register_prov_callback(prov_cb);
    esp_ble_mesh_register_custom_model_callback(custom_model_cb);
    err = esp_ble_mesh_init(&node_config, &composition);
    if (err) { ESP_LOGE(TAG, "Falha Mesh init: %d", err); return err; }

    /* Verifica se ja esta provisionado (dados persistidos no NVS) */
    if (esp_ble_mesh_node_is_provisioned()) {
        ESP_LOGI(TAG, "No ja provisionado (NVS). Restaurando estado...");
        mesh_ready = true;
    } else {
        err = esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV);
        if (err) { ESP_LOGE(TAG, "Falha ao ativar prov do no: %d", err); return err; }
        ESP_LOGI(TAG, "No BLE Mesh ativo. Aguardando provisionamento...");
    }
#else
    ESP_LOGW(TAG, "CONFIG_BLE_MESH desabilitado. Modo simulado.");
#endif

    memset(&last_rfid, 0, sizeof(last_rfid));
    memset(&battery_status, 0, sizeof(battery_status));
    return ESP_OK;
}

void k10_mesh_process_payload(const char *p)
{
    if (!p || !p[0]) return;
    char *j = strdup(p); if (!j) return;
    if (strstr(j, "\"type\":\"rfid\"")) {
        char t[64]={0},m[16]={0},nm[32]={0},l[16]={0}; float w=0;
        sscanf(j, "{\"type\":\"rfid\",\"model\":\"%15[^\"]\",\"tag\":\"%63[^\"]\",\"name\":\"%31[^\"]\",\"weight\":%f,\"lot\":\"%15[^\"]\"",m,t,nm,&w,l);
        if(t[0]){strncpy(last_rfid.tag,t,63);strncpy(last_rfid.model,m,15);strncpy(last_rfid.animal_name,nm,31);last_rfid.weight=w;strncpy(last_rfid.lot,l,15);last_rfid.valid=1;}
    } else if (strstr(j,"\"type\":\"batt\"")) {
        float v=0; int p=0;
        if(sscanf(j,"{\"type\":\"batt\",\"volt\":%f,\"pct\":%d",&v,&p)>=1){
            battery_status.voltage=v;
            battery_status.percentage=(uint8_t)p;
            battery_status.valid=1;
        }
    } else if (strstr(j, "\"type\":\"gps\"")) {
        double lat=0, lon=0; int fix=0; float alt=0, spd=0;
        if (sscanf(j, "{\"type\":\"gps\",\"lat\":%lf,\"lon\":%lf,\"fix\":%d,\"alt\":%f,\"speed\":%f",
                   &lat, &lon, &fix, &alt, &spd) >= 3) {
            gps_data.latitude = lat;
            gps_data.longitude = lon;
            gps_data.fix = (uint8_t)fix;
            gps_data.altitude = alt;
            gps_data.speed_kmh = spd;
            gps_data.valid = 1;
        }
    } else if (strstr(j, "\"type\":\"cell\"")) {
        int rssi=0, conn=0; char op[32]={0};
        if (sscanf(j, "{\"type\":\"cell\",\"rssi\":%d,\"connected\":%d,\"operator\":\"%31[^\"]\"",
                   &rssi, &conn, op) >= 2) {
            cell_status.rssi = rssi;
            cell_status.connected = (uint8_t)conn;
            strncpy(cell_status.operator_name, op, 31);
            cell_status.valid = 1;
        }
    } else if (strstr(j, "\"type\":\"status\"")) {
        int rfid_conn = 0, wifi_act = 0;
        if (sscanf(j, "{\"type\":\"status\",\"rfid_conn\":%d,\"wifi_active\":%d", &rfid_conn, &wifi_act) >= 2) {
            gateway_status.rfid_connected = (uint8_t)rfid_conn;
            gateway_status.wifi_active = (uint8_t)wifi_act;
            gateway_status.valid = 1;
        }
    }
    free(j);
}

k10_last_rfid_t* k10_mesh_get_last_rfid(void){return &last_rfid;}
k10_battery_status_t* k10_mesh_get_battery_status(void){return &battery_status;}
k10_alert_t* k10_mesh_get_alert(void){return &current_alert;}
k10_gps_data_t* k10_mesh_get_gps_data(void){return &gps_data;}
k10_cell_status_t* k10_mesh_get_cell_status(void){return &cell_status;}
k10_gateway_status_t* k10_mesh_get_gateway_status(void){return &gateway_status;}
void k10_mesh_clear_alert(void){memset(&current_alert,0,sizeof(current_alert));}

bool k10_mesh_is_ready(void){return mesh_ready;}

esp_err_t k10_mesh_send_accel(float ax,float ay,float az,uint8_t mov)
{
#if defined(CONFIG_BLE_MESH)
    if(!mesh_ready||!coord_addr) return ESP_ERR_INVALID_STATE;
    char json[128];
    snprintf(json,sizeof(json),"{\"type\":\"accel\",\"x\":%.2f,\"y\":%.2f,\"z\":%.2f,\"movement\":%d}",ax,ay,az,mov);
    esp_ble_mesh_msg_ctx_t ctx = {.net_idx=0,.app_idx=0,.addr=coord_addr,.send_ttl=4};
    esp_err_t ret = esp_ble_mesh_server_model_send_msg(&vnd_models[0],&ctx,OP_ACCEL,strlen(json),(uint8_t*)json);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Falha ao enviar accel: %d", ret);
    }
    return ret;
#else
    return ESP_OK;
#endif
}

esp_err_t k10_mesh_send_display_status(float battery_voltage, uint8_t battery_percentage,
                                        uint8_t critical, uint8_t screen_active,
                                        const char *ui_event)
{
#if defined(CONFIG_BLE_MESH)
    if(!mesh_ready||!coord_addr) return ESP_ERR_INVALID_STATE;
    char json[192];
    if (ui_event && ui_event[0]) {
        snprintf(json,sizeof(json),
                 "{\"type\":\"display\",\"volt\":%.2f,\"pct\":%u,\"crit\":%u,\"screen\":%u,\"event\":\"%s\"}",
                 battery_voltage, battery_percentage, critical, screen_active, ui_event);
    } else {
        snprintf(json,sizeof(json),
                 "{\"type\":\"display\",\"volt\":%.2f,\"pct\":%u,\"crit\":%u,\"screen\":%u}",
                 battery_voltage, battery_percentage, critical, screen_active);
    }
    esp_ble_mesh_msg_ctx_t ctx = {.net_idx=0,.app_idx=0,.addr=coord_addr,.send_ttl=4};
    esp_err_t ret = esp_ble_mesh_server_model_send_msg(&vnd_models[0],&ctx,OP_DISPLAY_STATUS,
                                                        strlen(json),(uint8_t*)json);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Falha ao enviar display status: %d", ret);
    }
    return ret;
#else
    return ESP_OK;
#endif
}
