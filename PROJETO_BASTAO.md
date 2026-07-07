# Sistema Bastao — Rastreabilidade Pecuária com RFID e GPS

## Visão Geral

O **Sistema Bastao** é um dispositivo de rastreamento pecuário que integra leitura de tags RFID (LF/UHF), GPS/GNSS multi-constelação, conectividade celular 4G e comunicação BLE Mesh com display K10. Projetado para operar em áreas rurais sem infraestrutura de rede Wi-Fi.

## Hardware

### Arquitetura Física

```
+---------------------+         UART2 (115200 8N1)        +---------------------+
|   STM32G070CBTx     |  PA2(TX) -> IO13(ESP32 RX)      |       ESP32-S3      |
|  (Sensing Hub)      |  PA3(RX) <- IO14(ESP32 TX)      |  (Connectivity Hub) |
|                     |  JSON sobre serial               |                     |
|  - WL-134 (LF RFID) |                                   |  - BLE Mesh (K10)   |
|  - YRM100 (UHF RFID)|                                   |  - BLE GATT (Mobile) |
|  - Bateria ADC (PB1)|                                   |  - SIMCom 7663E     |
|  - Buzzer (PB6)     |                                   |                     |
+---------------------+                                   +---------+-----------+
                                                                     |
                                                              UART2 (17/18, 115200)
                                                                     |
                                                          +----------v-----------+
                                                          |  SIMCom 7663E       |
                                                          |  4G/LTE Cat1        |
                                                          |  GPS/GLONASS/BDS/Gal|
                                                          |  Dual SIM (DSSS)    |
                                                          |  MQTT Nativo (AT)   |
                                                          |  Modem 7663E        |
                                                          +----------------------+
```

### Componentes Principais

| Componente | Função | Interface |
|-----------|--------|-----------|
| **STM32G070CBTx** | Leitor RFID, ADC bateria, buzzer | UART + GPIO |
| **ESP32-S3** | Conectividade, criptografia, orquestração | UART, BLE, SPI |
| **SIMCom 7663E** | Modem 4G + GPS/GNSS multi-constelação | UART AT commands |
| **WL-134** | Leitor RFID LF (134.2KHz) | UART 9600 8N2 |
| **YRM100** | Leitor RFID UHF | UART 115200 8N1 |
| **K10** | Display e interação | BLE Mesh |

### Pinagem STM32

| Pino | Periférico | Função |
|------|-----------|--------|
| PB1 (ADC1 IN9) | ADC | Medição bateria (divisor 100k/10k) |
| PA2/PA3 | USART2 | Comunicação com ESP32 (115200) |
| PA0/PA1 | USART4 | YRM100 UHF RFID (115200) |
| PA5/PB0 | USART3 | WL-134 LF RFID (9600 8N2) |
| PB4/PB5 | GPIO | Power control WL-134 / YRM100 |
| PB6 | GPIO | Buzzer |

### Pinagem ESP32

| GPIO | Conectado a | Direção | Função |
|------|------------|---------|--------|
| IO13 | STM32 PA2 (TX) | RX | UART1 RX do STM32 |
| IO14 | STM32 PA3 (RX) | TX | UART1 TX para STM32 |
| IO17 | SIMCom TX | TX | UART2 TX para modem |
| IO18 | SIMCom RX | RX | UART2 RX do modem |
| IO4 | SIMCom PWRKEY | OUT | Pulso de boot do modem |
| IO19 | Transistor NPN → NRST | OUT | Reset do STM32 (lógica invertida) |

### Reset Circuit (STM32)

```
GPIO19 (ESP32) ---> NPN Transistor ---> NRST (STM32)
  LOW  -> Transistor OFF -> NRST HIGH (pull-up) -> STM32 roda
  HIGH -> Transistor ON  -> NRST LOW           -> STM32 reset
```

---

## Firmware ESP32

### Stack de Conectividade (AT Commands)

O sistema utiliza **comandos AT puros** para toda comunicação com o modem SIMCom 7663E. **Não utiliza PPP** nem pilha TCP/IP do ESP-IDF para o caminho celular.

```
+------------------+       +------------------+       +------------------+
|  ESP32 Tasks     |       |  SIMCom 7663E    |       |     Broker       |
|                  |  AT   |                  |  TCP  |                  |
| dispatcher_task  |──────►│ AT+CMQTTSTART    |──────►│ MQTT Broker     |
| simcom_watchdog  |◄──────│ +CMQTTCONNECT:0  │◄──────│ :1883           |
| simcom_rx_task   |       │                  |       |                  |
|                  |       │ AT+CGNSSPWR=1    |       |                  |
| system_loop      |       │ AT+CGNSSINFO     |  GPS  |                  |
+------------------+       +------------------+       +------------------+
```

### FreeRTOS Tasks

| Task | Arquivo | Core | Prio | Stack | Função |
|------|---------|------|------|-------|--------|
| `stm32_uart_rx_task` | `stm32_uart.c` | 0 | 7 | 4096 | RX UART STM32, parse JSON |
| `dispatcher_task` | `main.c` | 0 | 6 | 6144 | Processa RFID, criptografa, publica |
| `main_loop` | `main.c` | 0 | 5 | - | Conectividade, GPS, heartbeat |
| `simcom_rx` | `simcom_driver.c` | 1 | 5 | 4096 | RX UART modem, URC parsing |
| `simcom_watchdog` | `simcom_driver.c` | 1 | 3 | 6144 | Watchdog SIMCom, SIM swap |
| `cache_sync` | `offline_cache.c` | 1 | 3 | 4096 | Sincronização cache offline |

### Fluxo de Dados (Tag RFID → Nuvem)

```
STM32 (Tag detectada)
  │
  ├── WL-134 (USART3, 9600) ──┐
  ├── YRM100 (USART4, 115200) ─┤
  │                            v
  │                    STM32 rfid_parser.c
  │                    JSON: {"type":"rfid","model":"YRM100","tag":"..."}
  │                            │
  │                    USART2 (115200)
  │                            v
  │                    ESP32 stm32_uart_rx_task
  │                    JSON parse → queue (50 slots)
  │                            │
  │                    dispatcher_task
  │                      ├── rfid_dedup_is_duplicate()
  │                      ├── animal_db_lookup() (nome, peso, lote)
  │                      ├── AES-256-CBC encrypt (secure_payload)
  │                      ├── mesh_coordinator_send_data() → K10
  │                      └── CELLULAR_ONLY?
  │                           ├── sim → simcom_driver_mqtt_publish()
  │                           └── não → mqtt_publisher_enqueue() → WiFi
  │                                    │
  │                                    v
  │                              Broker MQTT
  │                          agro/bastao/{MAC}/telemetry
```

### Módulos do Firmware ESP32

| Arquivo | Função |
|---------|--------|
| `main.c` | Orquestrador, dispatcher, manage_connectivity |
| `simcom_driver.c` | Driver SIMCom via AT commands, GPS, MQTT, SMS |
| `simcom_driver.h` | Header com structs e funções públicas |
| `stm32_uart.c` | UART STM32, JSON parser, watchdog |
| `stm32_cmd.c` | Comandos para STM32 (buzzer, RFID power) |
| `stm32_monitor.c` | Dashboard de saúde do STM32 |
| `mqtt_publisher.c` | MQTT via WiFi (esp_mqtt) - opcional |
| `wifi_driver.c` | Wi-Fi STA (só em modo WIFI ou AUTO) |
| `offline_cache.c` | Cache offline SPIFFS (FIFO) |
| `rfid_dedup.c` | Deduplicação RFID (64 entradas, 10s) |
| `secure_payload.c` | Criptografia AES-256-CBC |
| `ble_mobile.c` | GATT Server BLE para app mobile |
| `ble_mobile.h` | Structs: bastao_device_status_t, network_config_t |
| `mesh_coordinator.c` | Coordenador BLE Mesh |
| `esp_power.c` | Gerenciamento de energia (light/deep sleep) |
| `cmd_parser.c` | Parse de comandos remotos (MQTT/BLE) |
| `animal_db.c` | Banco de dados de animais (NVS) |
| `private_configs.h` | Configurações (gerado do .env) |

### Configuração (`private_configs.env` → `generate_config.py` → `private_configs.h`)

| Variável | Padrão | Descrição |
|----------|--------|-----------|
| `BASTAO_MQTT_URI` | `mqtt://209.50.240.55:1883` | Broker MQTT |
| `BASTAO_MQTT_CLIENT_ID` | `bastao-esp-001` | Client ID |
| `BASTAO_APN_NAME` | `iot.datatem.com.br` | APN 4G |
| `BASTAO_APN_USER` | `datatem` | Usuário APN |
| `BASTAO_APN_PASS` | `datatem` | Senha APN |
| `BASTAO_AES_KEY` | 64 hex chars | Chave AES-256 |
| `BASTAO_NET_MODE` | `cellular_only` | Modo de rede |
| `BASTAO_TOPIC_TELEMETRY` | `agro/bastao/{MAC}/telemetry` | Tópico telemetria |
| `BASTAO_TOPIC_GPS` | `agro/bastao/{MAC}/gps` | Tópico GPS |
| `BASTAO_WIFI_ENABLED` | `false` | Wi-Fi habilitado |

---

## Conexão Celular (SIMCom 7663E)

### Sequência de Inicialização

```
Boot:
  1. PWRKEY pulse (GPIO4 HIGH 2s) + aguarda 10s
  2. Cria task simcom_rx (UART escuta)
  3. at_init_sequence():
     a. AT (teste, 5s timeout)
     b. ATE1 (eco ativo)
     c. AT+UIMHOTSWAPLEVEL + AT+UIMHOTSWAPON
     d. Sonda slot 0: CFUN=0 → SELECTSIMSLOT=0 → CFUN=1 → CPIN? → CCID → CSQ
     e. Sonda slot 1: CFUN=0 → SELECTSIMSLOT=1 → CFUN=1 → CPIN? → CCID → CSQ
     f. Escolhe slot com melhor sinal (rssi)
     g. IMEI + MSISDN
     h. CREG=1, CGREG=1
  4. simcom_driver_configure_apn():
     a. wait_for_network_registration (60s timeout)
     b. CGATT=1, CGDCONT, CGAUTH
     c. Lê operadora (COPS) e sinal (CSQ)
  5. simcom_driver_mqtt_connect():
     a. CMQTTREL + CMQTTSTOP (cleanup sessão anterior)
     b. Delay 1s
     c. CMQTTSTART, CMQTTACCQ
     d. CMQTTWILLTOPIC + CMQTTWILLMSG
     e. CMQTTCONNECT (30s + 15s URC)
     f. SUBSCRIBE cmd + config (com delay 1s entre)
  6. GPS ligado (CGNSSPWR=1)
  7. Watchdog task inicia
```

### Comandos AT Principais

#### Inicialização
| Comando | Função | Timeout |
|---------|--------|---------|
| `AT` | Teste básico | 5s |
| `ATE1` | Eco ativo (debug) | 5s |
| `AT+CFUN=0` | Rádio off | 5s |
| `AT+CFUN=1` | Rádio on | 5s |
| `AT+CFUN=1,1` | Reset modem | 10s |
| `AT*SELECTSIMSLOT=<0\|1>` | Seleciona SIM (proprietário) | 5s |
| `AT+CPIN?` | Verifica SIM pronto | 5s |
| `AT+CICCID` | Lê ICCID | 5s |
| `AT+GSN` | Lê IMEI | 5s |
| `AT+CNUM` | Lê MSISDN | 9s |
| `AT+CREG=1` / `AT+CGREG=1` | URC de registro | 3s |
| `AT+CREG?` | Status registro | 3s |

#### Rede
| Comando | Função | Timeout |
|---------|--------|---------|
| `AT+CGATT=1` | GPRS attach | 10s |
| `AT+CGDCONT=1,"IP","<APN>"` | PDP context | 9s |
| `AT+CGAUTH=1,1,"user","pass"` | Autenticação PAP | 9s |
| `AT+COPS?` | Operadora atual | **65s** |
| `AT+CSQ` | Qualidade sinal | 9s |
| `AT+CPSI?` | Info sistema | 5s |

#### MQTT (SIMCom Nativo)
| Comando | Função | Timeout |
|---------|--------|---------|
| `AT+CMQTTSTART` | Inicia MQTT | 15s |
| `AT+CMQTTACCQ=0,"<id>"` | Adquire client ID | 5s |
| `AT+CMQTTWILLTOPIC=0,<len>` | Will topic | 5s |
| `AT+CMQTTWILLMSG=0,<len>,1` | Will message | 5s |
| `AT+CMQTTCONNECT=0,"tcp://..."` | Conecta broker | 30s |
| `AT+CMQTTSUBTOPIC=0,<len>,1` | Subscribe topic | 5s |
| `AT+CMQTTSUB=0` | Executa subscribe | 5s |
| `AT+CMQTTTOPIC=0,<len>` | Publish topic | 5s |
| `AT+CMQTTPAYLOAD=0,<len>` | Publish payload | 5s |
| `AT+CMQTTPUB=0,<qos>,60` | Executa publish | 10s |
| `AT+CMQTTDISC=0,120` | Desconecta | 5s |
| `AT+CMQTTREL=0` | Libera client | 3s |
| `AT+CMQTTSTOP` | Para MQTT | 15s |

#### GPS/GNSS
| Comando | Função | Timeout |
|---------|--------|---------|
| `AT+CGNSSPWR=1` | Liga GNSS | 9s |
| `AT+CGNSSPWR=0` | Desliga GNSS | 9s |
| `AT+CGPSINFO` | Posição GPS (NMEA DDMM.MMMM) | 9s |
| `AT+CGNSSINFO` | Posição GNSS (formato variável) | 9s |

#### SMS
| Comando | Função | Timeout |
|---------|--------|---------|
| `AT+CMGF=1` | Modo texto | 3s |
| `AT+CMGS="<num>"` | Enviar SMS | 5s prompt / **40s envio** |
| `AT+CMGL="REC UNREAD"` | Listar SMS | 9s |
| `AT+CMGD=<index>` | Deletar SMS | 9s |

### URCs (Unsolicited Result Codes)

| URC | Tratamento | Função |
|-----|-----------|--------|
| `+CMQTTCONNECT:<idx>,<err>` | ✅ `process_simcom_line()` | Resultado conexão MQTT |
| `+CMQTTCONNLOST:<idx>,<cause>` | ✅ | Conexão MQTT perdida |
| `+CMQTTNONET` | ✅ | Rede indisponível |
| `+CMQTTRXSTART:<idx>,<t_len>,<p_len>` | ✅ | Início mensagem recebida |
| `+CMQTTRXTOPIC:<idx>,<sub_t_len>` + dados | ✅ | Tópico da mensagem |
| `+CMQTTRXPAYLOAD:<idx>,<sub_p_len>` + dados | ✅ | Payload da mensagem |
| `+CMQTTRXEND:<idx>` | ✅ | Fim mensagem (processa) |
| `+CGNSSPWR:READY!` | ⚠️ Logado mas não tratado | GNSS pronto |
| `+CREG:<stat>` | ⚠️ Não tratado como URC | Mudança de registro |

## Sincronização de Horário

### Fonte única: Torre Celular (AT+CCLK)

O sistema **não utiliza SNTP** (NTP) porque o ESP32 não tem interface IP própria — o modem gerencia todo o TCP/IP internamente.

A sincronização é feita exclusivamente via torre celular:

```c
// No orchestrator_task, a cada ~30s enquanto hora < 2023
if (time(NULL) < 1700000000 && simcom_driver_get_state() >= SIMCOM_STATE_REGISTERED) {
    simcom_driver_sync_time_from_tower();
}
```

### Comando AT+CCLK?

```
AT+CCLK? → +CCLK: "26/06/16,08:32:59-16"
```

O campo `-16` é o timezone em **quarter-hours** (3GPP TS 27.007):
- `-16` = 16 × 15min = **4 horas** = GMT-4
- `-12` = 12 × 15min = **3 horas** = GMT-3

### Conversão

```
Torre: 08:32:59-16 → GMT-4 (08:32 AM)
tz_h = 16 → 16 × 900s = 4h
UTC = 08:32 + 4h = 12:32 UTC
TZ=AMT+4 → localtime: 12:32 - 4h = 08:32 GMT-4 ✅
```

### Timezone (TZ)

Configurado no boot para AMT+4 (America/Manaus, GMT-4):
```c
setenv("TZ", "AMT+4", 1);
tzset();
```

Usado nos formatos de timestamp com `%z`:
```c
strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%dT%H:%M:%S%z", tm_info);
// Resultado: 2026-06-16T08:32:59-0400
```

---

### Sondagem na inicialização
O `at_init_sequence()` sonda ambos os slots e escolhe o de melhor sinal:

```
Sonda slot 0: CFUN=0 → SELECTSIMSLOT=0 → CFUN=1 → CPIN? → CCID → CSQ
Sonda slot 1: CFUN=0 → SELECTSIMSLOT=1 → CFUN=1 → CPIN? → CCID → CSQ
Compara:
  - Ambos OK: escolhe o de maior RSSI (> menos negativo)
  - Só um OK: usa esse
  - Nenhum: fallback slot 0
```

### Troca inteligente (watchdog)
O watchdog (`simcom_watchdog_task`) troca de chip automaticamente quando:
- SIM ausente (`sim_present == false`) → troca imediata
- Sinal < -100dBm por 5 ciclos (50s) → troca
- MQTT desconectado por 180s + sem sinal → power cycle + troca

### NVS Persistence
Dados SIM salvos na NVS para recall no próximo boot:

| Chave | Conteúdo |
|-------|----------|
| `sim_imei` | IMEI do módulo |
| `sim0_msisdn` | Número slot 0 |
| `sim1_msisdn` | Número slot 1 |
| `sim0_ccid` | ICCID slot 0 |
| `sim0_oper` | Operadora slot 0 |
| `sim1_oper` | Operadora slot 1 |

---

## GPS/GNSS

### Funcionamento
- GNSS ligado no boot (`AT+CGNSSPWR=1`)
- Polling a cada **30s** pelo `system_orchestrator_task`
- Prioridade: **CGPSINFO** (NMEA DDMM.MMMM → decimal) com fallback CGNSSINFO
- GPS fix tracking com `bastao_current_status.gps_fix`

### Formato CGPSINFO (PRIMÁRIO - funciona)
```
+CGPSINFO: 2027.588218,S,05434.803006,W,150626,202805.000,502.08,0.290,0.00
lat=DDMM.MMMMMM → DD + MM.MMMMMM/60 = decimal
```

### Formato CGNSSINFO (FALLBACK - ASR1601)
```
+CGNSSINFO: 3,07,04,20.4597855,S,54.5801010,W,...
fix, GPS_SVs, GLO_SVs, lat, N/S, lon, E/W, ...
```

---

## MQTT

### Rotas de Publicação

```
CELLULAR_ONLY:
  dispatcher_task → simcom_driver_mqtt_publish(topic, payload, qos)
                   → AT+CMQTTTOPIC → AT+CMQTTPAYLOAD → AT+CMQTTPUB

WIFI/AUTO (WiFi ativo):
  dispatcher_task → mqtt_publisher_enqueue(topic, payload, qos)
                   → esp_mqtt_client_publish()
```

### Tópicos

| Tópico | Direção | Conteúdo |
|--------|---------|----------|
| `agro/bastao/{MAC}/telemetry` | Publish | RFID + GPS + bateria (criptografado AES) |
| `agro/bastao/{MAC}/gps` | Publish | Coordenadas GPS (reservado) |
| `id/{MAC}/cmd` | Subscribe | Comandos remotos |
| `id/{MAC}/config` | Subscribe | Configuração remota |

### Autenticação
- **Username**: MAC address (ex: `206EF1D4D574`)
- **Password**: Chave AES-256 em hex (64 chars)
- Enviados via `AT+CMQTTCONNECT`

---

## Atualização OTA (Over-the-Air)

### Visão Geral

O Sistema Bastão suporta atualização remota de firmware via OTA exclusivamente para o **ESP32 Coordenador**. O processo utiliza o componente `esp_https_ota` da ESP-IDF com partições duplas A/B (`ota_0`/`ota_1`) para garantir rollback seguro.

### Arquitetura de Partições

```
Partitions (ESP32 Coordinator):
  nvs       (24KB)  → Dados não voláteis
  otadata   (8KB)   → Estado do boot (qual partição bootar)
  ota_0     (1728K) → Slot de firmware A (ativo)
  ota_1     (1728K) → Slot de firmware B (inativo)
  spiffs    (512K)  → Cache offline
```

- O boot alterna entre `ota_0` e `ota_1` a cada OTA bem-sucedido.
- `esp_ota_mark_app_valid_cancel_rollback()` é chamado no boot para confirmar que o firmware é estável.

### Disparo

A atualização é disparada por comando MQTT no tópico `id/{MAC}/cmd`:

```json
{"cmd":"ota","url":"https://servidor-firmware.com/bastao_v2.0.bin"}
```

### Fluxo de Execução

1. Cloud envia comando OTA → MQTT → `cmd_parser_process_message()` → `ota_manager_start(url)`
2. `ota_manager_start()` cria `ota_task` (prio 4, Core 1, stack 8KB)
3. `ota_task` verifica se Wi-Fi está ativo (`esp_netif_is_netif_up()`)
4. Se Wi-Fi ativo: baixa binário via `esp_https_ota()`, grava na partição inativa
5. Se sucesso: `esp_restart()` após 3s
6. Se falha: loga erro, mantém firmware atual

### Limitações Conhecidas

| Limitação | Detalhe | Impacto |
|-----------|---------|---------|
| **Wi-Fi apenas** | `esp_https_ota` exige pilha TCP/IP LwIP. Modo CELLULAR_ONLY não tem interface IP local. | OTA não funciona via 4G. Necessário Wi-Fi (base carregadora/curral). |
| **Sem CI/CD** | Não há pipeline automatizado para build + upload do `.bin`. | O arquivo binário precisa ser gerado manualmente e hospedado em servidor HTTPS. |
| **Sem report de progresso** | `ota_task` não publica status (iniciando, baixando, erro, sucesso) de volta ao MQTT. | Cloud não sabe se OTA começou ou falhou. |
| **Sem FOTA do modem** | Comandos `AT+CFOTA` do SIMCom não implementados. | Firmware do módulo 4G só atualiza via porta serial. |
| **Sem OTA na K10** | K10 tem partição `factory` única, sem slots OTA. | Display K10 só atualiza via USB/serial físico. |

### Servidor de Firmware

O binário deve ser hospedado em servidor **HTTPS** com certificado SSL válido (o ESP32 usa `esp_crt_bundle_attach` para verificação). Exemplos:

- Servidor HTTP estático (Nginx, Apache, S3)
- GitHub Releases (https://github.com/user/repo/releases/download/vX.Y.Z/bastao.bin)
- Servidor dedicado na fazenda (rede local)

### Geração do Binário

```bash
# Na raiz do esp32_firmware:
idf.py build
# O arquivo de firmware está em:
# build/bastao_esp_coordinator.bin  (ou esp32_firmware/build/*.bin)
#
# Para uso com OTA, o binário correto é:
# build/ota_data_initial.bin  (apenas primeira gravação)
# O firmware completo está em esp32_firmware/build/bastao_esp_coordinator.bin
```

### Teste

```bash
python teste_automatizado/verify_ota.py
# Valida que o parser de comando {"cmd":"ota","url":"..."} funciona.
```

---

## Energia e Sleep

### STM32
- Sleep timeout: **120s** sem atividade
- Wake: UART activity (qualquer uma das 3 UARTs com stop mode habilitado)
- RFID readers desligados durante sleep
- Envia `{"type":"power","action":"sleep"}` para ESP32 antes de dormir
- Ao acordar: rearma `HAL_UART_Receive_IT` para todas as UARTs

### Ciclo de Sleep/Wake

```
1. 120s sem atividade → STM32 Power_Sleep()
2. STM32 envia "power sleep" JSON ao ESP32
3. STM32 desliga RFID readers
4. STM32 entra STOP1 (WFI), UART2/3/4 com wake habilitado
5. ESP32 recebe "power sleep" → seta stm32_sleeping=true
6. A cada 30s, ESP32 envia "\n" via UART para wake
7. STM32 acorda → reconfigura clock → religa readers → envia "power wake"
8. ESP32 recebe "power wake" → stm32_sleeping=false
```

### ESP32
- Light sleep timeout: **180s** (maior que STM32 para não perder mensagens de sleep)
- Wake: timer, GPIO ou UART
- Watchdog do STM32 envia `\n` periódico quando `stm32_sleeping=true` (não reseta)

---

## Cache Offline (SPIFFS)

### Funcionamento
- Armazenamento FIFO em SPIFFS (`/spiffs/msg_N.json`)
- Capacidade máxima: 95% do SPIFFS
- Tópico utilizado: **`bastao_network_config.mqtt.topic_telemetry`** (com MAC real)

### Fluxo
```
dispatcher: publish → falhou? → offline_cache_write(payload) ✅
sync_task (a cada 5s): MQTT conectado? → lê cache → publica no tópico correto
```

### Importante
O tópico usado na sincronização é o **mesmo** do dispatcher ao vivo: `agro/bastao/{MAC}/telemetry`. Antes da correção, o sync task usava tópicos hardcoded sem MAC. ✅

---

## Comunicação BLE Mesh (Tela K10)

O **Coordenador (ESP32)** e a **Tela K10** se se comunicam via rede local **BLE Mesh** usando a stack Bluedroid do ESP-IDF. A segurança da comunicação local é garantida nativamente pela criptografia da camada de link do BLE Mesh (AppKey + NetKey), trafegando payloads JSON em texto plano (sem encriptação AES-256-CBC redundante).

### Configuração do Vendor Model
Para evitar conflito com constantes internas do ESP-IDF (como `CID_NVAL = 0xFFFF` para modelos SIG), o projeto utiliza Company Identifier (CID) e Model Identifier (MID) personalizados:
- **Company ID (CID):** `0x02A5` (Espressif)
- **Model ID (MID):** `0x0001`
- **Opcodes do Vendor Model:**
  - `OP_RFID` (`0xC00001`): Coordenador → K10 (Leitura RFID / Telemetria)
  - `OP_ACCEL` (`0xC00002`): K10 → Coordenador (Dados do acelerômetro)
  - `OP_DISPLAY_STATUS` (`0xC00003`): K10 → Coordenador (Bateria, tela ativa, eventos UI)

### Capacidade de Segmentos (Segment Exhaustion Fix)
Para evitar falhas na fila de transmissão e erros de contextos de segmentos insuficientes (`err=-16` / "No multi-segment message contexts available") sob pacotes BLE de múltiplos segmentos, a configuração de filas concorrentes foi expandida de 1 para 4 em ambos os projetos (`sdkconfig.defaults`):
```ini
CONFIG_BLE_MESH_TX_SEG_MSG_COUNT=4
CONFIG_BLE_MESH_RX_SEG_MSG_COUNT=4
```

### Formato de Payloads (Coordenador → K10)
A comunicação trafega payloads estruturados em formato JSON simples:

1. **Mensagem de Leitura RFID (Tag lida no STM32):**
   ```json
   {"type":"rfid","model":"YRM100","tag":"982000211311029","name":"Mimoso","weight":450.50,"lot":"Lote A"}
   ```
2. **Mensagem de GPS (Enviado a cada 30 segundos):**
   ```json
   {"type":"gps","lat":-20.449785,"lon":-54.580101,"fix":1,"alt":502.1,"speed":0.5}
   ```
3. **Mensagem de Status Celular (Enviado a cada 60 segundos):**
   ```json
   {"type":"cell","rssi":-75,"connected":1,"operator":"Vivo"}
   ```
4. **Mensagem de Status Geral (Enviado a cada 30 segundos):**
   ```json
   {"type":"status","rfid_conn":1,"wifi_active":1}
   ```
   Indica se as antenas RFID controladas pelo STM32 estão operacionais (`rfid_conn`) e se o modo de rede do Coordenador possui suporte a Wi-Fi/Dual (`wifi_active`).

### Exibição de Ícones Dinâmicos na Tela K10
Os ícones do cabeçalho superior da K10 atualizam-se dinamicamente conforme os status recebidos:
- **RFID:** Inicia em vermelho no boot e só fica verde se o STM32 reportar heartbeat ativo (`rfid_conn == 1`).
- **WiFi:** Ocultado no boot; só é exibido na tela se o modo de rede ativo no Coordenador for Wi-Fi ou Dual/Auto (`wifi_active == 1`).
- **4G:** Ocultado no boot; fica visível em verde exibindo a potência do sinal (ex: `"4G -85dBm"`) quando o modem celular estiver conectado. É ocultado sob perda de sinal.
- **GPS:** Mostra `"GPS NÃO SINCRONIZADO"` em vermelho no boot ou quando não houver sinal (`fix == 0`). O ícone e a coordenada só ficam verdes ao obter fixação.

### Desacoplamento de Bateria (K10 Battery Source Shift)
- A leitura de bateria do STM32 é ignorada pelo Coordenador para evitar loops beeps de alarmes errados.
- A bateria da tela K10 (0-100%) passa a ser a única fonte oficial para o payload MQTT da nuvem (`nivel_bateria`).
- O Coordenador sincroniza seu status de bateria interna utilizando a leitura enviada pela K10 (`OP_DISPLAY_STATUS`).

### Sincronismo Imediato de Sensores (RFID Scan Roundtrip)
Para garantir que os dados de bateria e acelerômetro enviados à nuvem no momento exato do scan de um animal estejam atualizados:
1. Ao receber a tag RFID via BLE Mesh (`OP_RFID`), a Tela K10 imediatamente lê o acelerômetro e a bateria locais e os envia de volta para o Coordenador.
2. O Coordenador, após encaminhar a tag para a Mesh, realiza um atraso controlado de `300ms` (`vTaskDelay`) antes de gerar o payload MQTT. Isso fornece a janela de tempo necessária para a recepção dos sensores atualizados.

### Persistência de Rede e Reconexão (NVS)
Para evitar perda de comunicação após desligamento ou reinicialização (onde o Coordenador esquecia os nós e o K10 parava de enviar beacons de pareamento por se considerar já pareado):
1. **Configuração NVS:** Ambos os firmwares compilam com a persistência de configurações habilitada:
   ```ini
   CONFIG_BLE_MESH_SETTINGS=y
   ```
2. **Restauração de Estado:** Durante a inicialização do Coordenador, logo após o bind do modelo local, o banco de dados do provisionador no NVS é verificado na faixa de endereços reservados (`0x0005` a `0x0010`):
   ```c
   for (uint16_t addr = 0x0005; addr < 0x0010; addr++) {
       esp_ble_mesh_node_t *node = esp_ble_mesh_provisioner_get_node_with_addr(addr);
       if (node != NULL) {
           k10_addr = addr;
           k10_provisioned = true;
           break;
       }
   }
   ```
   Caso o nó seja encontrado na tabela interna, a comunicação é restabelecida instantaneamente no boot sem necessidade de novo pareamento físico.

---

## Tela K10 (Display + BLE Mesh Node)

### Hardware
- **MCU:** ESP32-S3 (unihiker)
- **Display:** LVGL 8.x, 480x480, SPI (TFT + Touch)
- **Speaker:** NS4168 via I2S (2W, sine wave, canal desabilitado entre beeps)
- **I/O Expander:** XL9535 (I2C 0x20) — backlight, botões
- **RFID:** Recebe tags via BLE Mesh do Coordenador
- **Flash:** 16MB (partições: factory 3MB + SPIFFS 2MB)

### Pinagem K10

| Pino | Função | Nota |
|------|--------|------|
| GPIO0 | I2S BCLK | Speaker NS4168 |
| GPIO38 | I2S LRCK | Speaker NS4168 |
| GPIO45 | I2S SDO (TX) | Speaker NS4168 |
| — | MCLK | NC (não conectado) |
| GPIO46 | WS2812 | RGB LED (3 LEDs) |
| GPIO47 | I2C SDA | XL9535, Acelerômetro |
| GPIO48 | I2C SCL | XL9535, Acelerômetro |

### Funcionalidades K10

| Feature | Descrição |
|---------|-----------|
| **Tela Brinco Lido** | Tela branca fullscreen "BRINCO LIDO" + tag, auto-dismiss 3s |
| **Speaker I2S** | Beep ao ler tag via speaker NS4168 (short/long/double/alert) |
| **SPIFFS Contador** | Banco de tags diárias `/spiffs/tags/YYYY-MM-DD.json` |
| **Aba Histórico** | Lista das últimas 20 tags lidas (Tab2) |
| **GPS Display** | Coordenadas reais do Coordenador via Mesh |
| **Bateria Display** | Percentual 0-100% via Mesh (não do STM32) |
| **Backlight** | Timeout 120s, wake por botão ou tag RFID |

### Tags para MQTT (via Mesh)

O Coordenador envia tags à nuvem **somente se tiver localização** (GPS fix OU cell tower):
- **Sem localização:** tag é enviada à K10 via Mesh (exibição), mas descartada do MQTT
- **Cell tower fallback:** se GPS sem fix, usa triangulação via Mozilla Location Service

### Partições K10

```
factory  (3MB)  → Firmware K10
spiffs   (2MB)  → Banco de tags diárias
nvs      (24KB) → Configurações BLE Mesh
```

---

## Segurança

### Criptografia
- Algoritmo: **AES-256-CBC** com PKCS#7 padding
- Chave: 32 bytes (64 hex chars) do `private_configs.env`
- IV: Gerado aleatoriamente a cada payload
- Payloads MQTT e Mesh criptografados antes do envio

### BLE
- Autenticação obrigatória para escrita em características sensíveis
- GATT server com UUIDs personalizados

---

## Compilação

### STM32
```bash
# No Windows com STM32CubeCLT + Make
cd stm32_firmware
make -j4
```

### ESP32
```bash
# Requer ESP-IDF v5.5.2
cd esp32_firmware
pip install -r requirements.txt
python generate_config.py          # Atualiza private_configs.h
idf.py set-target esp32s3
idf.py build
idf.py -p COMx flash monitor
```

### Dependências
- ESP-IDF v5.5.2
- Python 3.9+
- ARM GCC toolchain (para STM32)
- CMake + Ninja
