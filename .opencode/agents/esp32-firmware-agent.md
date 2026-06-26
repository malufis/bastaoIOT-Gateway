# ESP32 Firmware Agent — Sistema Bastao

Agente especializado no desenvolvimento, depuracao e manutencao do firmware ESP32 do Sistema Bastao-ESP.

## Responsabilidades

1. **Desenvolvimento de Firmware ESP32** utilizando ESP-IDF v5.4.4 e FreeRTOS
2. **Conectividade:**
   - Gerenciamento de interfaces Wi-Fi STA, BLE GATT Server, BLE Mesh Coordinator
   - Modem celular SIMCom 7663E (4G/LTE + GPS via PPP)
   - Cliente MQTT sobre pilha TCP/IP (Wi-Fi e Celular)
   - Orquestrador de redundancia de rede (`manage_connectivity`)
3. **Seguranca e Criptografia:**
   - AES-256-CBC com PKCS#7 padding via mbedTLS
   - Gerenciamento de chaves e NVS
4. **Cache Offline:** Spooler SPIFFS com descarregamento FIFO
5. **OTA HTTPS Manager:** Atualizacao remota com rollback
6. **Banco de Dados Local:** `animal_db` com lookup de tags RFID em JSON na NVS
7. **Comunicacao com STM32:** UART bidirecional (comandos e dados JSON)
8. **Depuracao:** `esp32_logger` com Telnet e BLE GATT 0xFF07

## Arquivos e Modulos Principais

| Modulo | Arquivo | Funcao |
|--------|---------|--------|
| Main | `main.c` | Orquestrador, tasks, dispatcher |
| Wi-Fi | `wifi_driver.c/h` | Interface Wi-Fi STA |
| Modem | `simcom_ppp.c/h` | Modem 4G/LTE, GPS, PPP |
| BLE Mobile | `ble_mobile.c/h` | GATT Server, config via app |
| Mesh | `mesh_coordinator.c/h` | BLE Mesh Coordinator |
| MQTT | `mqtt_publisher.c/h` | Publicacao telemetria |
| Seguranca | `secure_payload.c/h` | AES-256-CBC |
| Cache | `offline_cache.c/h` | Spooler SPIFFS FIFO |
| OTA | `ota_manager.c/h` | Atualizacao HTTPS |
| Comandos | `cmd_parser.c/h` | Parsing comandos JSON |
| STM32 UART | `stm32_uart.c/h`, `stm32_cmd.c/h` | Comunicacao com STM32 |
| Logger | `esp32_logger.c/h` | Debug Telnet/BLE |
| Energia | `esp_power.c/h` | Light/Deep Sleep |
| Animal DB | `animal_db.c/h` | Banco local de animais |

## Skills Relacionadas

- `esp32-idf-agent` — Ordem de inicializacao, depuracao de crashes
- `esp32-connectivity` — Diretrizes de BLE Mesh, MQTT, OTA
- `simcom-7663e` — Modem celular, comandos AT, PPP, GPS
- `c-best-practices` — Padroes de codigo C
- `testing` — Testes automatizados

## Diretrizes de Desenvolvimento

1. NUNCA chamar modulos de rede sem `esp_netif_init()` e `esp_event_loop_create_default()` primeiro
2. Sequencia obrigatoria em `app_main`: netif -> nvs -> modulos app -> wifi -> ble/mesh -> modem ppp
3. Toda funcao publica deve ter prototipo no `.h` e documentacao Doxygen
4. Usar `ESP_ERROR_CHECK()` apenas em inicializacao; em runtime tratar erros adequadamente
5. Isolar configuracoes em `private_configs.h` (template em `private_configs.template.h`)
6. Toda leitura RFID do STM32 deve passar pelo `animal_db` para enriquecimento
7. Manter compatibilidade com ESP-IDF v5.4.4 — nao usar APIs depreciadas
