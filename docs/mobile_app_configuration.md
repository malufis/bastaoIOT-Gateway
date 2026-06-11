# Análise: Configuração via Aplicativo Mobile

## 1. Funcionalidades Implementadas ✅

### 1.1 GATT Server BLE (ble_mobile.c/h)

| Característica UUID | Descrição | Status |
|---------------------|-----------|--------|
| 0xFF01 | Config Write (App → Bastão) | ✅ Implementado |
| 0xFF02 | Config Read (Bastão → App) | ✅ Implementado |
| 0xFF03 | Business Write (Fazenda/Lote/Animal) | ✅ Implementado |
| 0xFF04 | Last Tag Notify | ✅ Implementado |
| 0xFF05 | Device Status Read (Bateria, GPS, Movimento, SIM status) | ✅ Implementado |
| 0xFF06 | Cellular Status Read/Notify (Sinal, Tecnologia, Operador) | ✅ Implementado |
| 0xFF07 | Debug Logger BLE (Streaming de Logs em Tempo Real) | ✅ Implementado |

### 1.2 Dados de Configuração Salvos no NVS

| Dado | Chave NVS | Tipo | Status |
|------|-----------|------|--------|
| Potência YRM100 | `yrm_power` | uint8 | ✅ Implementado |
| Tempo de varredura | `scan_time` | uint16 | ✅ Implementado |
| Intervalo bateria | `battery_report_interval` | uint8 | ✅ Implementado |
| WL-134 habilitado | `wl134_enabled` | bool | ✅ Implementado |
| YRM100 habilitado | `yrm100_enabled` | bool | ✅ Implementado |
| SSID Wi-Fi | `wifi_ssid` | string | ✅ Implementado |
| Senha Wi-Fi | `wifi_pass` | string | ✅ Implementado |
| Wi-Fi habilitado | `wifi_en` | bool | ✅ Implementado |
| APN Celular | `apn` | string | ✅ Implementado |
| Usuário APN | `apn_user` | string | ✅ Implementado |
| Senha APN | `apn_pass` | string | ✅ Implementado |
| Celular habilitado | `cell_en` | bool | ✅ Implementado |
| Broker MQTT URI | `mqtt_uri` | string | ✅ Implementado |
| Cliente MQTT ID | `mqtt_client` | string | ✅ Implementado |
| MQTT Tópico Telemetria | `mqtt_tele` | string | ✅ Implementado |
| MQTT Tópico GPS | `mqtt_gps` | string | ✅ Implementado |
| Modo de Rede (Auto/WiFi/Cell) | `net_mode` | uint8 | ✅ Implementado |
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

### 2.2 Configuração de Rede via App (Implementado)

| Dado | Necessário | Status |
|------|------------|--------|
| SSID Wi-Fi | ✅ Sim | ✅ Concluído |
| Senha Wi-Fi | ✅ Sim | ✅ Concluído |
| APN Celular | ✅ Sim | ✅ Concluído |
| Usuário APN | ✅ Sim | ✅ Concluído |
| Senha APN | ✅ Sim | ✅ Concluído |
| Broker MQTT URI | ✅ Sim | ✅ Concluído |
| MQTT Tópico Telemetria | ✅ Sim | ✅ Concluído |
| MQTT Tópico GPS | ✅ Sim | ✅ Concluído |
| Cliente MQTT ID | ✅ Sim | ✅ Concluído |
| Modo de rede (Wi-Fi/Celular/Auto) | ✅ Sim | ✅ Concluído |

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
  },
  "mode": 3
}
```
*Modos: 0 (WiFi Only), 1 (Cellular Only), 2 (WiFi/Cellular Dual), 3 (Auto)*

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

## 4. Estrutura de Configuração e NVS no Código (ble_mobile.h)

### 4.1 Chaves NVS Mapeadas
```c
#define NVS_NAMESPACE_CONFIG     "bastao_cfg"
#define NVS_KEY_YRM100_POWER     "yrm_power"
#define NVS_KEY_SCAN_TIME        "scan_time"
#define NVS_KEY_WIFI_SSID        "wifi_ssid"
#define NVS_KEY_WIFI_PASSWORD    "wifi_pass"
#define NVS_KEY_WIFI_ENABLED     "wifi_en"
#define NVS_KEY_APN              "apn"
#define NVS_KEY_APN_USER         "apn_user"
#define NVS_KEY_APN_PASSWORD     "apn_pass"
#define NVS_KEY_CELLULAR_ENABLED "cell_en"
#define NVS_KEY_MQTT_URI         "mqtt_uri"
#define NVS_KEY_MQTT_CLIENT_ID   "mqtt_client"
#define NVS_KEY_MQTT_TOPIC_TELE  "mqtt_tele"
#define NVS_KEY_MQTT_TOPIC_GPS   "mqtt_gps"
#define NVS_KEY_NETWORK_MODE     "net_mode"
```

### 4.2 Estrutura de Dados
```c
typedef struct {
    wifi_config_t wifi;
    cellular_config_t cellular;
    mqtt_config_t mqtt;
    network_mode_t mode;
} network_config_t;
```

---

## 5. Status do Roadmap de Configuração

| Item | Status no Roadmap | Detalhes |
|------|-------------------|----------|
| GATT Server BLE | ✅ Concluído | Serviços e características 0xFF01-0xFF07 operacionais. |
| Dados de negócio (Farm/Lote/Animal) | ✅ Concluído | Lookup local enriquecido. |
| Configuração de SSID/Senha Wi-Fi | ✅ Concluído | Persistência na NVS via JSON BLE. |
| APN de Celular | ✅ Concluído | Configuração dinâmica do chip. |
| Parâmetros de Broker MQTT | ✅ Concluído | Tópicos e Broker configurados pelo App. |
| Modo de Rede (Auto/WiFi/Cellular) | ✅ Concluído | Comutação dinâmica no loop do main.c. |

---

## 6. Histórico de Implementação

Todas as configurações são recebidas via pacotes JSON pela característica GATT `0xFF01` ou pelo tópico MQTT `id/<ID>/config` (onde `<ID>` é o IMEI do modem celular), gravadas de forma atômica no armazenamento NVS Flash do ESP32 e aplicadas imediatamente via `ble_mobile_apply_network_config()`, reiniciando os subsistemas de conectividade correspondentes (reconectando Wi-Fi, reiniciando o publisher MQTT ou alterando a suspensão do modem celular).

---

## 7. Métricas Celulares Estendidas e Backup SMS (Fase 26) ✅

### 7.1 Métricas de Sinal Estendidas (GATT 0xFF06 / JSON)
A característica GATT `0xFF06` e os status JSON foram estendidos para retornar informações de antenas LTE quando disponíveis:
- **RSRP (Reference Signal Received Power):** Sinal em dBm.
- **RSRQ (Reference Signal Received Quality):** Qualidade em dB.
- **SINR (Signal-to-Interference-plus-Noise Ratio):** Razão sinal-ruído em dB.
- **CEER (Extended Error Report):** Mensagem textual de log de erro de rede gerada pelo modem celular.

Para evitar interrupções de dados, o driver utiliza **cacheamento automático**. Se a sessão celular PPP estiver ativa, os dados de antena retornados são os últimos valores salvos em cache; se o link estiver offline (modo de comandos AT), uma consulta direta de rádio (`AT+CPSI?` e `AT+CEER`) é feita em tempo real.

### 7.2 Motor de SMS de Contingência (Backup)
Quando a conexão de dados PPP está inativa, o watchdog de conectividade no ESP32 executa o polling do modem para ler mensagens não lidas (`AT+CMGL="REC UNREAD"`).
Os seguintes comandos são aceitos (tanto em texto plano quanto em formato JSON `{"cmd": "..."}`):
1. **BUZZER:** Aciona padrão de bipe duplo no buzzer do STM32.
2. **RFID ON / RFID OFF:** Liga/desliga o driver e alimentação física do leitor RFID.
3. **STATUS:** Retorna SMS com a telemetria atual do Bastão (tensão da bateria, status PPP/MQTT, contagem de tags lidas, chip SIM ativo, CCID e coordenadas de GPS com fix).
4. **RESTART:** Envia confirmação e reinicia o ESP32 (`esp_restart()`).

Após o processamento de cada mensagem, ela é removida da memória do modem (`AT+CMGD`) para liberar espaço de armazenamento.

---

*Documento atualizado em: 2026-05-21*
*Versão: 2.1*