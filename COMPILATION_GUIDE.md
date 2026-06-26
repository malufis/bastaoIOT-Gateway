# Guia de Compilação — Sistema Bastao

## Pré-requisitos

### STM32 Firmware
- **STM32CubeCLT** (GNU Tools for STM32 12.3.rel1)
- **Make** (GNU Make 4.x)
- **ARM GCC Toolchain** (`arm-none-eabi-gcc`)

### ESP32 Coordinator Firmware
- **ESP-IDF v5.4.4** (Espressif IoT Development Framework)
- **Python 3.9+**
- **CMake** + **Ninja**
- **Git**

### K10 Display Firmware
- **ESP-IDF v5.4.4** (mesmo ambiente do coordinator)
- **LVGL** (biblioteca gráfica, inclusa)
- **Componentes:** LCD ILI9341, touch FT6236, acelerômetro SC7A20

---

## 1. Configuração Inicial

### 1.1 Gerar `private_configs.h`

O arquivo `main/private_configs.h` é **automaticamente gerado** a partir de `private_configs.env`:

```bash
cd esp32_firmware
python generate_config.py
```

Isso lê as variáveis do `.env` e gera os defines C. **Sempre execute após alterar o `.env`.**

### 1.2 Arquivo `.env`

Edite `private_configs.env` com as configurações do seu dispositivo:

```env
# Broker MQTT
BASTAO_MQTT_URI=mqtt://209.50.240.55:1883
BASTAO_MQTT_CLIENT_ID=bastao-esp-001

# APN 4G
BASTAO_APN_NAME=iot.datatem.com.br
BASTAO_APN_USER=datatem
BASTAO_APN_PASS=datatem

# AES Key (64 hex chars - 32 bytes)
BASTAO_AES_KEY=0123456789ABCDEFFEDCBA98765432101032547698BADCFEEFCDAB8967452301

# Modo de Rede: auto | wifi_only | cellular_only
BASTAO_NET_MODE=cellular_only

# Wi-Fi (opcional, fallback)
BASTAO_WIFI_SSID=
BASTAO_WIFI_PASS=
BASTAO_WIFI_ENABLED=false

# Tópicos MQTT ({MAC} substituído automaticamente pelo MAC real)
BASTAO_TOPIC_TELEMETRY=agro/bastao/{MAC}/telemetry
BASTAO_TOPIC_GPS=agro/bastao/{MAC}/gps
```

---

## 2. Compilar STM32

```bash
cd stm32_firmware/Debug

# Compilar (usa makefile gerado pelo STM32CubeIDE)
make -j4

# Saída: bastao-Stm32.elf
```

**Limpar build:**
```bash
make clean
```

**Gravando:**
- Use STM32CubeProgrammer para gravar o `.elf` ou `.hex`
- Conecte via SWD (ST-Link) ou UART (DFU mode)

---

## 3. Compilar ESP32

### 3.1 Configurar ESP-IDF

```bash
# No Windows (PowerShell):
C:\Espressif\frameworks\esp-idf-v5.4.4\export.ps1

# Ou defina manualmente:
$env:IDF_PATH = "C:\Espressif\frameworks\esp-idf-v5.5.2"
```

### 3.2 Gerar config + Build

```bash
cd esp32_firmware

# 1. Gerar private_configs.h (sempre que alterar .env)
python generate_config.py

# 2. Configurar target (primeira vez apenas)
idf.py set-target esp32s3

# 3. Limpar build anterior (se necessário)
idf.py fullclean

# 4. Compilar
idf.py build

# 5. Gravar e monitorar
idf.py -p COMx flash monitor

# Para sair do monitor: Ctrl+]
```

### 3.3 Configurações do sdkconfig

Principais opções:
```
CONFIG_LWIP_PPP_SUPPORT=y          # PPP não usado mas compilado
CONFIG_SPIFFS_SUPPORT=y            # Cache offline
CONFIG_BT_ENABLED=y                # BLE Mesh + GATT
CONFIG_BT_NIMBLE_ENABLED=n         # Usar Bluedroid (padrão)
CONFIG_PARTITION_TABLE_CUSTOM=y    # Partições OTA
```

---

## 4. Compilar K10

```bash
cd k10_firmware

# 1. Configurar target (primeira vez apenas)
idf.py set-target esp32s3

# 2. Compilar
idf.py build

# 3. Gravar (erase-flash recomendado ao mudar CID do vendor model)
idf.py -p COM5 erase-flash flash monitor

# Para sair do monitor: Ctrl+]
```

### 4.1 Configurações do sdkconfig (K10)

```
CONFIG_BLE_MESH=y                  # BLE Mesh Node
CONFIG_BLE_MESH_SETTINGS=y         # Persistência NVS da subnet
CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MANUAL=y
CONFIG_LVGL_ENABLE=y               # LVGL GUI
```

---

## 6. Limpeza de NVS

Para resetar configurações salvas na NVS (útil quando alterar modo de rede):

```bash
idf.py erase_flash
```

Ou apenas apagar a partição NVS:
```bash
idf.py erase_partition nvs
```

---

## 7. Estrutura de Diretórios

```
Bastao-ESP/
├── esp32_firmware/           # Firmware ESP32 (ESP-IDF)
│   ├── main/                 # Código fonte
│   │   ├── main.c            # Orquestrador principal
│   │   ├── simcom_driver.c   # Driver SIMCom (AT commands)
│   │   ├── stm32_uart.c      # Comunicação STM32
│   │   ├── mqtt_publisher.c  # MQTT WiFi (opcional)
│   │   ├── ble_mobile.c      # BLE GATT Server
│   │   ├── mesh_coordinator.c# BLE Mesh Coordinator
│   │   ├── secure_payload.c  # AES-256-CBC
│   │   ├── offline_cache.c   # Cache SPIFFS
│   │   ├── private_configs.h # GERADO - não editar
│   │   └── rfid_dedup.c      # Deduplicação RFID
│   ├── private_configs.env   # Configurações (EDITAR)
│   ├── generate_config.py    # Gerador do .h
│   └── sdkconfig.defaults    # Configurações IDF
│
├── k10_firmware/             # Firmware Tela K10 (ESP-IDF + LVGL)
│   ├── main/                 # main.c, gui_task, network_task
│   ├── components/           # Componentes customizados
│   │   ├── k10_mesh/         # BLE Mesh Node (receptor RFID, GPS, cell)
│   │   ├── gui/              # LVGL (telas, temas, sensores)
│   │   ├── hal_display/      # LCD ILI9341 + touch FT6236
│   │   ├── hal_sensors/      # Acelerômetro SC7A20
│   │   └── hal_buttons/      # Botões físicos
│   └── sdkconfig.defaults    # Configurações IDF
│
├── stm32_firmware/           # Firmware STM32 (CubeIDE)
│   ├── Core/Src/             # Código fonte
│   │   ├── main.c            # Loop principal
│   │   ├── rfid_parser.c     # Parse YRM100 + WL-134
│   │   ├── battery_monitor.c # ADC bateria
│   │   ├── power_mgmt.c      # Sleep/wake
│   │   └── alerts.c          # Buzzer e alertas
│   └── Debug/                # Build artifacts
│
├── k10_firmware/             # Firmware Tela K10 (ESP-IDF)
├── Manual/                   # Documentação técnica
├── docs/                     # Documentação do projeto
└── aprendizado/              # Arduino sketches de referência
```

---

## 5. OTA (Over-the-Air) — Preparação do Binário

### Gerar o Firmware para OTA

```bash
cd D:\git\Bastao\Bastao-ESP\esp32_firmware
idf.py build
```

O binário de firmware completo estará em:
```
esp32_firmware/build/bastao_esp_coordinator.bin
```

Ou genericamente em:
```
esp32_firmware/build/*.bin
```

> **Importante:** Use o arquivo `.bin` completo (não o `ota_data_initial.bin`, que é apenas para primeira gravação via serial).

### Hospedagem

O binário deve ser hospedado em servidor **HTTPS** com certificado SSL válido. O ESP32 usa `esp_crt_bundle_attach` para verificar o certificado.

Exemplos de servidores compatíveis:
- GitHub Releases: `https://github.com/user/repo/releases/download/v1.0/bastao.bin`
- Nginx/Apache estático
- Servidor local na fazenda (rede Wi-Fi)

### Disparo

Envie comando MQTT para `id/{MAC}/cmd`:
```json
{"cmd":"ota","url":"https://servidor.com/bastao_v2.0.bin"}
```

### Limitações

- **Wi-Fi é obrigatório**: OTA via 4G não funciona (vide documentação técnica).
- **K10 não suporta OTA**: Display K10 requer gravação via USB/serial.
- **Sem report de progresso**: O status do OTA não é publicado de volta ao MQTT.

---

## 6. Troubleshooting

### Log nível DEBUG
```c
// Em main.c, ajustar níveis de log:
esp_log_level_set("SIMCOM_DRV", ESP_LOG_DEBUG);    // AT commands
esp_log_level_set("SIMCOM_DRV", ESP_LOG_INFO);     // Só resultados
esp_log_level_set("SIMCOM_DRV", ESP_LOG_WARN);     // Só erros
```

### Erro: "Fila de publicacao MQTT nao inicializada"
**Causa:** Modo CELLULAR_ONLY sem `mqtt_publisher_init()` (esperado).
**Solução:** O dispatcher e offline cache já usam `simcom_driver_mqtt_publish()` diretamente.

### Erro: "AT Timeout" no primeiro AT
**Causa:** Modem não respondeu no tempo configurado.
**Verificar:**
1. Polaridade do PWRKEY (GPIO4 HIGH 2s → LOW)
2. Baud rate (115200)
3. Alimentação do modem (3.8V típico)

### Erro: MQTT subscribe erro 14 ("client is busy")
**Causa:** Segundo subscribe enviado sem delay.
**Solução:** Delay de 1s entre subscribes já implementado.
