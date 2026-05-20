# Análise: Configuração via Aplicativo Mobile

## 1. Funcionalidades Implementadas ✅

### 1.1 GATT Server BLE (ble_mobile.c/h)

| Característica UUID | Descrição | Status |
|---------------------|-----------|--------|
| 0xFF01 | Config Write (App → Bastão) | ✅ Implementado |
| 0xFF02 | Config Read (Bastão → App) | ✅ Implementado |
| 0xFF03 | Business Write (Fazenda/Lote/Animal) | ✅ Implementado |
| 0xFF04 | Last Tag Notify | ✅ Implementado |
| 0xFF05 | Device Status Read | ✅ Implementado |

### 1.2 Dados de Configuração Salvos no NVS

| Dado | Chave NVS | Tipo | Status |
|------|-----------|------|--------|
| Potência YRM100 | `yrm_power` | uint8 | ✅ Implementado |
| Tempo de varredura | `scan_time` | uint16 | ✅ Implementado |
| Intervalo bateria | `battery_report_interval` | uint8 | ✅ Implementado |
| WL-134 habilitado | `wl134_enabled` | bool | ✅ Implementado |
| YRM100 habilitado | `yrm100_enabled` | bool | ✅ Implementado |
| Dados da Fazenda | `farm_data` | JSON | ✅ Implementado |
| Dados do Lote | `lot_data` | JSON | ✅ Implementado |
| Dados dos Animais | `animal_data` | JSON | ✅ Implementado |

---

## 2. Dados Mutáveis - O que está no código (Hardcoded)

### 2.1 Configurações de Rede (main.c)

```c
// Wi-Fi - HARDCODED
wifi_driver_connect("bastaoIOT", "3spB@st@0");

// APN - HARDCODED
static const simcom_apn_config_t default_apn = {
    .apn = "zap.vivo.com.br",
    .user = "vivo",
    .password = "vivo",
};

// MQTT - HARDCODED
static const mqtt_publisher_config_t default_mqtt_config = {
    .broker_uri = "mqtt://broker.example.com:1883",
    .topic_telemetry = "bastao/telemetria",
    .topic_gps = "bastao/gps",
    .client_id = "bastao-esp-001",
};
```

### 2.2 O que FALTA configurar via App

| Dado | Necessário | Status |
|------|------------|--------|
| SSID Wi-Fi | ✅ Sim | ❌ Falta |
| Senha Wi-Fi | ✅ Sim | ❌ Falta |
| APN | ✅ Sim | ❌ Falta |
| Usuário APN | ✅ Sim | ❌ Falta |
| Senha APN | ✅ Sim | ❌ Falta |
| Broker MQTT URI | ✅ Sim | ❌ Falta |
| MQTT Tópico Telemetria | ✅ Sim | ❌ Falta |
| MQTT Tópico GPS | ✅ Sim | ❌ Falta |
| Cliente MQTT ID | ✅ Sim | ❌ Falta |
| Modo de rede (Wi-Fi/Celular) | ✅ Sim | ❌ Falta |

---

## 3. Estrutura Sugerida para o App

### 3.1 Tela de Configurações de Rede

```json
{
  "wifi": {
    "ssid": "bastaoIOT",
    "password": "3spB@st@0",
    "enabled": true
  },
  "cellular": {
    "apn": "zap.vivo.com.br",
    "apn_user": "vivo",
    "apn_password": "vivo",
    "enabled": true
  }
}
```

### 3.2 Tela de Configurações MQTT

```json
{
  "mqtt": {
    "broker_uri": "mqtt://broker.example.com:1883",
    "topic_telemetry": "bastao/telemetria",
    "topic_gps": "bastao/gps",
    "client_id": "bastao-esp-001"
  }
}
```

### 3.3 Tela de Configurações de Hardware

```json
{
  "hardware": {
    "yrm100_power": 20,
    "scan_time_ms": 200,
    "battery_report_interval_s": 5,
    "wl134_enabled": true,
    "yrm100_enabled": true
  }
}
```

---

## 4. Recomendação de Implementação

### 4.1 Alterações necessárias no ble_mobile.h

Adicionar novas chaves NVS:
```c
#define NVS_KEY_WIFI_SSID       "wifi_ssid"
#define NVS_KEY_WIFI_PASSWORD  "wifi_pass"
#define NVS_KEY_APN            "apn"
#define NVS_KEY_APN_USER      "apn_user"
#define NVS_KEY_APN_PASSWORD  "apn_pass"
#define NVS_KEY_MQTT_URI       "mqtt_uri"
#define NVS_KEY_MQTT_CLIENT    "mqtt_client"
```

### 4.2 Nova estrutura de configuração

```c
typedef struct {
    char wifi_ssid[32];
    char wifi_password[64];
    bool wifi_enabled;

    char apn[64];
    char apn_user[32];
    char apn_password[32];
    bool cellular_enabled;

    char mqtt_broker[128];
    char mqtt_client_id[32];
    char mqtt_topic_telemetry[64];
    char mqtt_topic_gps[64];
} network_config_t;
```

---

## 5. Resumo - O que está no Roadmap

| Item | Status no Roadmap | Prioridade |
|------|-------------------|------------|
| GATT Server BLE | ✅ Phase 11 (concluído) | - |
| Dados de negócio (Farm/Lote/Animal) | ✅ Phase 11 (concluído) | - |
| SSID/Senha Wi-Fi | ❌ Não está no roadmap | ALTA |
| APN configuração | ❌ Não está no roadmap | ALTA |
| Broker MQTT | ❌ Não está no roadmap | ALTA |
| Seleção de rede (Wi-Fi/Celular) | ⚠️ Phase 14 (parcial) | MÉDIA |

---

## 6. Ação Necessária

O projeto precisa de uma nova fase para **configuração de rede via aplicativo**:

### Phase 21 sugerida: Configuração de Rede via App Mobile

- [ ] Adicionarchar wifi_ssid/senha ao ble_mobile
- [ ] Adicionar APN ao ble_mobile
- [ ] Adicionar MQTT ao ble_mobile
- [ ] Criar sistema de apply de config (reiniciei componentes quando receber novo config)
- [ ] Testar configuração via app

---

*Documento gerado em: 2026-05-20*
*Versão: 1.0*