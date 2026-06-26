# Infraestrutura e Docker

## Backend (Python FastAPI)

O backend do Sistema Bastao e composto por 8 servicos Docker orquestrados:

| Servico | Funcao | Porta |
|---------|--------|:-----:|
| `api` | FastAPI (REST) | 8000 |
| `receptor` | Coletor MQTT (paho-mqtt) | - |
| `agentes` | Agentes de background (conectividade, BMS, SIM) | - |
| `web` | Frontend React SPA (Nginx) | 80 |
| `sicar_worker` | Worker de integracao SICAR | - |
| `extrator` | Extrator de dados externos | - |
| `postgres` | PostgreSQL + PostGIS | 5432 |
| `redis` | Cache e filas | 6379 |

### Docker Compose

```yaml
networks:
  bastao-net:
    driver: bridge

services:
  api:
    build: ./api
    ports: ["8000:8000"]
    depends_on: [postgres, redis]
    networks: [bastao-net]

  receptor:
    build: ./receptor
    depends_on: [postgres, redis, api]
    networks: [bastao-net]

  postgres:
    image: postgis/postgis:15-3.3
    environment:
      POSTGRES_DB: bastao
      POSTGRES_PASSWORD: ${DB_PASSWORD}
    volumes: [pgdata:/var/lib/postgresql/data]
    networks: [bastao-net]
```

## Firmware Embedded

### ESP32 Coordinator (`esp32_firmware/`)

- **Framework:** ESP-IDF v5.4.4
- **Target:** ESP32-S3
- **Particoes:** A/B para OTA (app0: 0x20000, app1: 0x1D0000)
- **Flash:** 4MB
- **SPIFFS:** Cache offline (msgs JSON)

### K10 Display (`k10_firmware/`)

- **Framework:** ESP-IDF v5.4.4 + LVGL v8
- **Target:** ESP32-S3
- **Display:** ILI9341 (240x320, SPI 40MHz)
- **Backlight:** XL9535 (I2C expander)
- **Flash:** Particao unica (sem OTA)

### STM32 (`stm32_firmware/`)

- **Framework:** STM32CubeIDE HAL (bare-metal)
- **MCU:** STM32G070CBTx (Cortex-M0+, 64MHz)
- **Flash:** 128KB
- **RAM:** 36KB
- **Build:** Makefile (Release) ou STM32CubeIDE

## Comandos de Build

### ESP32
```powershell
cd esp32_firmware
idf.py set-target esp32s3
idf.py build
idf.py -p COM7 flash monitor
```

### K10
```powershell
cd k10_firmware
idf.py set-target esp32s3
idf.py build
idf.py -p COM5 erase-flash flash monitor
```

### STM32
```powershell
$env:Path = "C:\ST\STM32CubeCLT_1.16.0\GNU-tools-for-STM32\bin;$env:Path"
cd stm32_firmware\Release
& "C:\ST\STM32CubeIDE_1.16.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.1.300.202402091052\tools\bin\make.exe" main-build
```

## Variaveis de Ambiente

Arquivo `private_configs.env` na raiz do projeto:

```env
# Modem
NET_MODE=cellular_only
APN=zap.vivo.com.br
APN_USER=**
APN_PASS=**

# MQTT
MQTT_BROKER=ssl://broker.bastao.com:8883
MQTT_USER=**
MQTT_PASS=**

# Seguranca
AES_KEY_HEX=0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF
```
