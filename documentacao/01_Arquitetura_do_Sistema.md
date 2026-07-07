# Arquitetura do Sistema

## Fluxo de Dados

```
[Tag RFID]
     |
     v
STM32 (RFID Parser)
  |-- WL-134 (LF): UART3 9600 8N2, frame 30 bytes, checksum XOR
  |-- YRM100 (UHF): UART4 115200 8N1, frame 0xBB...0x7E, poll 100ms
     |
     | JSON via UART2 (115200)
     v
ESP32 - stm32_uart_rx_task (Core 1, prio 6)
     |
     v
ESP32 - dispatcher_task (Core 0, prio 6)
  |-- rfid_dedup_is_duplicate() (janela 60s)
  |-- animal_db_lookup()
  |-- GPS gate: has_location()?
  |    ├── GPS fix → ok
  |    ├── Cell tower → ok (fallback via Mozilla Location Service)
  |    └── Sem localização → descarta do MQTT (envia à K10 via Mesh)
  |-- mesh_coordinator_send_data()  -> BLE Mesh -> K10
  |-- mqtt_publisher_enqueue_raw()  -> mqtt_pub_task
                                      -> secure_payload_encrypt() (AES-256-CBC)
                                      -> simcom_driver_mqtt_publish() (AT+CMQTTPUB)
                                      |-> se falha: offline_cache (SPIFFS)
```

## Tasks FreeRTOS (ESP32 Coordinator)

| Task | Prio | Core | Funcao |
|------|:----:|:----:|--------|
| `dispatcher_task` | 6 | 0 | Processa tags RFID (dedup, lookup, Mesh, MQTT) |
| `stm32_uart_rx_task` | 6 | 1 | RX serial do STM32 |
| `simcom_rx` | 6 | 1 | RX serial do modem SIMCom |
| `system_orchestrator` | 5 | 0 | Orquestracao: GPS, time, status |
| `gps_reader` | 3 | 1 | Leitura GPS via AT (bloqueante, em background) |
| `mqtt_pub_task` | 4 | 1 | Publicacao MQTT (AES + publish) |
| `simcom_wd` | 3 | 1 | Watchdog de conexao celular |
| `cache_sync_task` | 3 | 1 | Sincronizacao de cache offline |
| `ota_task` | 4 | 1 | Atualizacao OTA (evento raro) |
| `log_processor` | 3 | 1 | Processamento de logs |
| `telnet_logger` | 3 | 1 | Logger via Telnet |

## Tasks FreeRTOS (K10 Display)

| Task | Prio | Core | Funcao |
|------|:----:|:----:|--------|
| `gui_task` | 5 | 1 | LVGL + sensores + mesh polling (50ms) |
| `network_task` | 1 | 0 | BLE Mesh node (ociosa, stack roda em callbacks) |

## Funcionalidades K10

| Feature | Descrição |
|---------|-----------|
| Speaker I2S | Beep ao ler tag via NS4168 (short/long/double/alert) |
| Tela Brinco Lido | Exibição fullscreen "BRINCO LIDO" + tag, auto-dismiss 3s |
| SPIFFS Contador | Banco de tags diárias `/spiffs/tags/YYYY-MM-DD.json` |
| Aba Histórico | Lista das últimas 20 tags lidas (Tab2) |
| GPS Display | Coordenadas reais do Coordenador via Mesh |
| Bateria Display | Percentual 0-100% via Mesh (não do STM32) |
| Backlight | Timeout 120s, wake por botão ou tag RFID |

## Comunicacao BLE Mesh

**Coordenador (ESP32) <-> Node (K10)**

- **Vendor Model:** CID=0x02A5 (Espressif), MID=0x0001
- **Opcodes:**
  - `0xC00001` (OP_RFID): Coordenador -> K10 (tag RFID, GPS, cell, status)
  - `0xC00002` (OP_ACCEL): K10 -> Coordenador (acelerometro)
  - `0xC00003` (OP_DISPLAY_STATUS): K10 -> Coordenador (bateria, eventos UI)
- **Seguranca:** Criptografia nativa do BLE Mesh (AppKey + NetKey)
- **Payload:** JSON em texto plano (sem AES-256-CBC redundante)
- **Persistencia:** NVS (CONFIG_BLE_MESH_SETTINGS=y)
- **Segmentos:** TX_SEG_MSG_COUNT=12, RX_SEG_MSG_COUNT=12

### Formatos de Payload

**RFID Tag:**
```json
{"type":"rfid","model":"YRM100","tag":"300833B2DDD9014000000000","name":"Boi 123","weight":450.0,"lot":"LOTE01"}
```

**GPS:**
```json
{"type":"gps","lat":-20.4442,"lon":-54.6194,"fix":1,"alt":350.0,"speed":0.5}
```

**Status Celular:**
```json
{"type":"cell","rssi":-75,"connected":1,"operator":"VIVO"}
```

**Gateway Status:**
```json
{"type":"status","rfid_conn":1,"wifi_active":0}
```

**Bateria:**
```json
{"type":"batt","volt":8.85,"pct":85}
```

## Comunicacao com Modem SIMCom (AT Commands)

Toda a pilha de rede e gerenciada pelo modem via comandos AT nativos. O ESP32 envia comandos AT via UART serial.

### GPS (A-GPS + Cold Start)

O GPS utiliza **A-GPS** (`AT+CAGPS`) para acelerar o fix inicial. Com dados de assistência do servidor AGNSS baixados via 4G, o Time to First Fix é reduzido de **30s+ para 2-5s**.

- **Inicialização:**
  1. `AT+CGNSSPWR=1` (power on GNSS, cold start)
  2. Aguarda URC `+CGNSSPWR:READY!` (~9s no chip ASR1601)
  3. `AT+CGNSSMODE=7` (GPS+BDS+GLONASS — multi-constelação)
  4. `AT+CAGPS` (download efeméride/almanac via socket TCP 4G, 9s timeout)
  5. `AT+CGPSCOLD` (reinicia GPS com dados de assistência)

- **Polling adaptativo:**
  - Sem fix: a cada **2 segundos** (busca rapida)
  - Com fix: a cada **30 segundos** (economia)
- **Task separada:** `gps_reader_task` (prio 3, Core 1) executa `simcom_driver_get_gps()` em background, acionada via `xTaskNotify`. O `sys_orchestr` nunca bloqueia.
- **Fallback para cold start puro:** Se `AT+CAGPS` falhar (sem 4G, erro 101-105), GPS opera em cold start puro.
- **Formato principal:** `AT+CGPSINFO` (NMEA DDMM.MMMM), fallback `AT+CGNSSINFO`.

### Cell Tower Location (Cache Assíncrono)

Quando GPS não tem fix, o sistema usa triangulação por torre celular (Mozilla Location Service). Para não bloquear o processamento RFID, a consulta HTTP é feita em background:

- **Cache assíncrono:** `sys_orchestr` atualiza lat/lon da torre a cada **5 minutos**
- **Dispatcher:** Lê valor em cache (~0ms), sem esperar HTTP (10s)
- **Invalidação automática:** Quando o Cell ID muda (handover/roaming)
- **Só atualiza se GPS não tem fix** (evita HTTP desnecessário)

### Boot Sequence
```
AT -> ATE1 -> CFUN=0 -> CFUN=1,1 -> SELECTSIMSLOT -> CPIN? -> CICCID
-> GSN (IMEI) -> CNUM (MSISDN) -> CREG? -> CGATT=1 -> CGDCONT=1
-> CGAUTH=1,0,user,pass -> COPS? -> CSQ
-> CGNSSPWR=1 -> aguarda READY! -> CGNSSMODE=7 -> CAGPS (A-GPS)
-> CMQTTSTART -> CMQTTACCQ -> CMQTTCONNECT -> CMQTTSUB -> Watchdog
```

### Timeouts Críticos
| Comando | Timeout | Nota |
|---------|:-------:|------|
| CFUN=1,1 | 10s | Modem reset |
| COPS? | 65s | Busca operadora |
| CMQTTSTART | 15s | Inicio MQTT |
| CMQTTCONNECT | 30s | Conexao MQTT |
| CMQTTPUB | 10s | Publicacao QoS 1 |
| CGNSSPWR | 9s | Power-on GPS |
| CGNSSMODE | 5s | Configura constelações |
| CAGPS | 9s | Download A-GPS via 4G |
| CGPSINFO | 9s | Leitura GPS |
| CMGS (SMS) | 40s | Envio SMS |

## Comunicacao STM32 <-> ESP32

- **UART2** (STM32): PA2(TX), PA3(RX), 115200 8N1
- **UART1** (ESP32): IO13(RX), IO14(TX), 115200 8N1
- **Formato:** JSON com ``\n`` como terminador

### Mensagens STM32 -> ESP32
```json
{"type":"rfid","model":"YRM100","tag":"300833B2DDD9014000000000"}
{"type":"rfid","model":"WL134","tag":"000123456789012345678901"}
{"type":"batt","volt":8.85}
{"type":"heartbeat"}
{"type":"alert","code":"batt_low","volt":9.20}
{"type":"yrm100_cfg","fw":"2.1.3","region":2,"region_name":"US","power":20}
```

### Comandos ESP32 -> STM32
```json
{"cmd":"buzzer","type":"short"}
{"cmd":"rfid_on"}/{"cmd":"rfid_off"}
{"cmd":"yrm_power","value":1}/{"cmd":"yrm_power","value":0}
{"cmd":"yrm_restart"}
```
