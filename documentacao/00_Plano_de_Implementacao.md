# Plano de Implementacao — Sistema Bastao

## Visao Geral

O **Sistema Bastao** e um dispositivo de rastreamento pecuario que integra leitura de tags RFID (LF/UHF), GPS/GNSS multi-constelacao, conectividade celular 4G e comunicacao BLE Mesh com display K10. Projetado para operar em areas rurais sem infraestrutura de rede Wi-Fi.

## Stack Tecnologica

| Componente | Tecnologia | Versao |
|-----------|-----------|--------|
| **MCU Leitor RFID** | STM32G070CBTx (ARM Cortex-M0+) | STM32CubeIDE HAL |
| **MCU Conectividade** | ESP32-S3 (Xtensa LX7 dual-core) | ESP-IDF v5.4.4 |
| **Display K10** | ESP32-S3 + LVGL v8 | ESP-IDF v5.4.4 |
| **Modem 4G + GPS** | SIMCom A7670C/A7663E | AT commands puros |
| **RFID LF** | WL-134 (134.2KHz, FDX-B) | UART 9600 8N2 |
| **RFID UHF** | YRM100 (902-928MHz) | UART 115200 8N1 |
| **App Mobile** | React Native / Expo | BLE GATT |
| **Backend** | Python FastAPI | PostgreSQL + PostGIS |

## Arquitetura Fisica

```
+---------------------+    UART2 (115200 8N1)    +---------------------+
|   STM32G070CBTx     | PA2->IO13, PA3<-IO14    |      ESP32-S3       |
|  (Sensing Hub)      |=========================| (Connectivity Hub)  |
|                     |    JSON sobre serial     |                     |
|  - WL-134 (LF RFID) |                          |  - BLE Mesh (K10)   |
|  - YRM100 (UHF RFID)|                          |  - BLE GATT (Mobile) |
|  - Bateria ADC (PB1)|                          |  - SIMCom 7663E     |
|  - Buzzer (PB6)     |                          |                     |
+---------------------+                          +---------+-----------+
                                                             |
                                                      UART (17/18, 115200)
                                                             |
                                                  +----------v-----------+
                                                  |  SIMCom 7663E       |
                                                  |  4G/LTE Cat1        |
                                                  |  GPS/GLONASS/BDS/Gal|
                                                  |  Dual SIM (DSSS)    |
                                                  |  MQTT Nativo (AT)   |
                                                  +---------------------+
```

## Fases de Implementacao

### Fase 1 — Hardware e Firmware Base (Concluido)
- Configuracao STM32: GPIO, UART, ADC, interrupcoes
- Leitura WL-134 (LF RFID) com buffer circular
- Comunicacao serial STM32 <-> ESP32

### Fase 2 — Conectividade (Concluido)
- Modem SIMCom: inicializacao, comandos AT, MQTT nativo
- Dual SIM com watchdog e troca por sinal
- GPS multi-constelacao com polling adaptativo

### Fase 3 — BLE Mesh (Concluido)
- Coordenador ESP32 + Node K10
- Provisionamento com persistencia NVS
- Comunicacao JSON plano (sem criptografia extra)

### Fase 4 — Display K10 (Concluido)
- LVGL com icones de status condicionais
- Backlight com timeout 2min e wake por botao/tag
- Acelerometro e bateria

### Fase 5 — YRM100 UHF (Concluido)
- Single poll a cada 100ms
- Configuracao: regiao US 902-928MHz, potencia 20dBm
- Leitura de configuracao para debug via ESP32

### Fase 6 — Otimizacoes (Concluido)
- GPS em task separada (prio 3, Core 1)
- Prioridades FreeRTOS ajustadas
- WiFi AP com WPA2 para debug Telnet

### Proximas Fases (Planejadas)
- OTA via 4G
- OTA na K10
- CI/CD com report de progresso

## Hardware e Pinagem

### STM32G070CBTx (LQFP48)

| Pino | Funcao | Conectado a |
|------|--------|-------------|
| PA0 | USART4_TX | YRM100 RX |
| PA1 | USART4_RX | YRM100 TX |
| PA2 | USART2_TX | ESP32 IO13 (RX) |
| PA3 | USART2_RX | ESP32 IO14 (TX) |
| PA5 | USART3_TX | WL-134 (nao usado) |
| PA8 | MCO | Clock output |
| PB0 | USART3_RX | WL-134 TX |
| PB1 | ADC1_IN1 | Divisor bateria (100k/10k) |
| PB3 | WL134_PWR | Power switch WL-134 |
| PB4 | YRM100_PWR | Power switch YRM100 (ativo alto) |
| PB5 | YRM100_PWR_CTRL | Controle de alimentacao YRM100 |
| PB6 | BUZZER | Sinalizador sonoro |
| PC14 | LED_STATUS | Indicador de status |
| PC15 | - | (reservado) |

### ESP32-S3

| GPIO | Funcao | Conectado a |
|------|--------|-------------|
| IO13 | UART1_RX | STM32 PA2 (TX) |
| IO14 | UART1_TX | STM32 PA3 (RX) |
| IO17 | UART2_TX | SIMCom RX |
| IO18 | UART2_RX | SIMCom TX |
| GPIO42 | RGB LED K10 | Indicador visual |

## Modos de Rede

- **CELLULAR_ONLY** (padrao): Apenas 4G, MQTT via comandos AT
- **WIFI_ONLY**: Apenas Wi-Fi (quando na base)
- **AUTO**: Prefere Wi-Fi, fallback 4G

Wi-Fi AP ``Bastao-XXXXXX`` (WPA2, senha ``bastao123``) e sempre iniciado para debug Telnet.
