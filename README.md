# Sistema Bastao — Rastreabilidade Pecuária

Dispositivo de rastreamento pecuário com leitura RFID (LF/UHF), GPS multi-constelação, conectividade 4G/LTE via comandos AT, BLE Mesh para display K10 e BLE GATT para app mobile.

## Stack Tecnológica

| Componente | Tecnologia |
|-----------|-----------|
| **MCU Leitor RFID** | STM32G070CBTx (STM32CubeIDE) |
| **MCU Conectividade** | ESP32-S3 (ESP-IDF v5.4.4) |
| **Display K10** | ESP32-S3 (ESP-IDF v5.4.4 + LVGL + BLE Mesh Node) |
| **Modem 4G + GPS** | SIMCom 7663E (AT commands puros) |
| **RFID LF** | WL-134 (134.2KHz, UART 9600) |
| **RFID UHF** | YRM100 (UHF, UART 115200) |
| **App Mobile** | React Native / Expo (BLE GATT) |
| **Backend** | Python FastAPI + PostgreSQL + PostGIS |

## Arquitetura de Conectividade

**Comandos AT puros (sem PPP).** O modem SIMCom 7663E gerencia nativamente TCP/IP + MQTT. O ESP32 envia comandos AT via UART serial e o modem executa toda a pilha de rede.

```
ESP32 ──UART (17/18)── SIMCom 7663E
         │
         ├── AT+CGNSSPWR=1   (GPS ligado)
         ├── AT+CGPSINFO     (lê posição)
         ├── AT+CMQTTSTART   (MQTT via comando AT)
         ├── AT+CMQTTPUB     (publica tags)
         └── AT+CSQ          (sinal)

BLE Mesh ── display K10
         │
         ├── JSON plain text (rfid, batt, gps, cell)
         ├── Recebe accel, display_status da K10
         └── CID=0x02A5 (vendor model Espressif)
```

## Funcionalidades Principais

- ✅ Leitura RFID LF (WL-134) e UHF (YRM100) com deduplicação
- ✅ GPS/GLONASS/BeiDou/Galileo (multi-constelação)
- ✅ MQTT nativo via comandos AT (sem PPP)
- ✅ Dual SIM com troca inteligente por sinal
- ✅ Cache offline (SPIFFS) com sincronização automática
- ✅ Criptografia AES-256-CBC (payload MQTT)
- ✅ BLE Mesh Coordinator ↔ Node (display K10)
- ✅ BLE GATT Server (app mobile)
- ✅ Atualização OTA via Wi-Fi
- ✅ Gerenciamento de energia (sleep mode)
- ✅ Display K10: RFID, GPS, status celular, acelerômetro, bateria
- ✅ Comunicação BLE Mesh em JSON simples (sem criptografia extra)
- ⚠️ **OTA via 4G** não suportado (requer Wi-Fi)
- ❌ **OTA na tela K10** não suportado (apenas USB/serial)

## Modos de Rede

- **CELLULAR_ONLY** (padrão): Apenas 4G, sem Wi-Fi
- **WIFI_ONLY**: Apenas Wi-Fi (quando na base)
- **AUTO**: Prefere Wi-Fi, fallback 4G

## Início Rápido

### ESP32 Coordinator
```bash
cd esp32_firmware
# 1. Configurar
python generate_config.py
# 2. Compilar e gravar
idf.py set-target esp32s3
idf.py build
idf.py -p COM7 flash monitor
```

### K10 Display
```bash
cd k10_firmware
idf.py set-target esp32s3
idf.py build
idf.py -p COM5 erase-flash flash monitor
```

### STM32
```bash
cd stm32_firmware/Debug
make -j4
```

## Fluxo de Dados

```
STM32 (RFID) ──UART──┐
                     v
              dispatcher_task (ESP32)
                ├── JSON plain → BLE Mesh → K10 (RFID, GPS, Cell)
                ├── AES-256-CBC encrypt → MQTT → Nuvem
                └── Cache offline (se MQTT offline)

K10 Display:
  ┌─ RFID tag, animal name
  ├─ GPS coordinates (6 casas decimais)
  ├─ 4G status (verde/vermelho)
  ├─ Accelerometer data
  └─ Battery status
```

## Documentação

- [`documentacao/00_Plano_de_Implementacao.md`](documentacao/00_Plano_de_Implementacao.md) — Visão geral, stack, fases, pinagem
- [`documentacao/01_Arquitetura_do_Sistema.md`](documentacao/01_Arquitetura_do_Sistema.md) — Fluxo de dados, tasks, protocolos, BLE Mesh
- [`documentacao/02_Infraestrutura_e_Docker.md`](documentacao/02_Infraestrutura_e_Docker.md) — Build, deploy, Docker, variáveis de ambiente
- [`documentacao/03_Multi_Tenancy_e_Seguranca.md`](documentacao/03_Multi_Tenancy_e_Seguranca.md) — Criptografia, BLE, MQTT, multi-tenancy
- [`documentacao/04_Agentes_e_Desenvolvimento.md`](documentacao/04_Agentes_e_Desenvolvimento.md) — Agents, skills, padrões de código, git
- [`documentacao/08_Roadmap.md`](documentacao/08_Roadmap.md) — Cronograma, fases, próximos passos
- [`AGENTS.md`](AGENTS.md) — Diário de desenvolvimento completo
- [`COMPILATION_GUIDE.md`](COMPILATION_GUIDE.md) — Guia de compilação detalhado
- [`docs/k10_communication_protocol.md`](docs/k10_communication_protocol.md) — Protocolo BLE Mesh K10
- [`docs/k10_integration_guide.md`](docs/k10_integration_guide.md) — Guia de integração K10
