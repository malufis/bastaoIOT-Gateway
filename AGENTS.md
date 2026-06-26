# Diário de Desenvolvimento — Sistema Bastao

---

## Sessão 01 — Correção do Sistema de Bateria
**Data:** 2026-06-15
**Objetivo:** Calibrar leitura de tensão da bateria STM32.

**Problemas Resolvidos:**
- Tensão mostrava ~7.66V mas multímetro marcava 8.87V
- Adicionado `BATTERY_DIVIDER_RATIO` configurável em `battery_monitor.h`
- Ajustado fator para 12.74

**Modificações:**
- `stm32_firmware/Core/Inc/battery_monitor.h`: `BATTERY_DIVIDER_RATIO`
- `stm32_firmware/Core/Src/battery_monitor.c`: usa o define

---

## Sessão 02 — GPS/GLONASS: Análise Arduino vs ESP-IDF
**Data:** 2026-06-15
**Objetivo:** Analisar comandos GPS do Arduino rastreadorSimcom.ino e comparar.

**Aprendizado:**
- Arduino usa `AT+CGNSSPWR=1` (comando correto SIMCom multi-constelação)
- Arduino não usa PPP (sempre modo comando AT)
- Tempo de timeout 30s para GPS power-on
- Documentação SIMCom A76XX confirma: CGNSSPWR timeout máximo 9000ms

**Modificações:**
- `simcom_ppp.c` (arquivo deletado - mudanças perdidas)
- Descoberto que projeto já migrou para AT commands puros (Fase 42)

---

## Sessão 03 — Correção Watchdog STM32 + Timezone + Modo Rede
**Data:** 2026-06-15
**Objetivo:** Corrigir watchdog falso positivo, timezone GMT-4, modo CELLULAR_ONLY.

**Problemas Resolvidos:**
- ESP32 light sleep timeout (60s) menor que STM32 sleep (120s) → watchdog acionava falso positivo
- Timezone hardcoded `-03:00` → `-04:00`
- Wi-Fi sendo inicializado mesmo em CELLULAR_ONLY

**Modificações:**
- `esp_power.h`: light sleep timeout 60s → 180s
- `stm32_uart.c`: flag `stm32_sleeping`, retorna `alive` quando dormindo
- `main.c`: timestamp `-03:00` → `-04:00`
- `main.c`: wifi_driver_init() só roda se não for CELLULAR_ONLY
- `ble_mobile.c`: NETWORK_MODE_AUTO → CELLULAR_ONLY
- `private_configs.env`: auto → cellular_only
- `stm32_monitor.h/.c`: campos rfid_duplicates, rfid_unique_total

---

## Sessão 04 — RFID: Buffer Overflow + Poll Adaptativo + Log Duplicatas
**Data:** 2026-06-15
**Objetivo:** Corrigir lentidão na leitura das antenas RFID.

**Problemas Resolvidos:**
- Buffer circular YRM100 de 128 bytes sem proteção overflow (aumentado para 512)
- Poll YRM100 a cada 200ms incondicional (agora só envia se buffer vazio, timeout 500ms)
- Sem contagem de duplicatas no monitor

**Modificações (STM32):**
- `rfid_parser.h`: RFID_BUFFER_SIZE 128 → 512
- `rfid_parser.c`: overflow protection + RFID_GetOverflowCount()
- `main.c` (STM32): poll condicional `!RFID_HasData(1) || timeout > 500`

**Modificações (ESP32):**
- `rfid_dedup.c`: total_duplicates, total_unique, per-tag read_count
- `main.c` dispatcher: chama stm32_monitor_count_duplicate/unique
- `stm32_uart.c`: xQueueCreate 20 → 50

---

## Sessão 05 — GPS: Parser CGNSSINFO Bug + Timeouts + SIM Swap
**Data:** 2026-06-15/16
**Objetivo:** Corrigir GPS que nunca obtinha fix.

**Problemas Resolvidos:**
- Parser CGNSSINFO esperava 5 campos SV (fix_mode, GPS, GLO, BDS, GAL) mas modem só envia 4 → lat/lon interpretados como SV count
- Timeouts insuficientes (doc diz 9000ms, código usava 5000ms)
- `at_send_cmd` semáforo não drenado entre comandos
- `AT+COPS?` timeout 5s mas doc diz 60s

**Descobertas crÍticas:**
- Modem A7670C-BASS_DTU usa chip GPS ASR1601
- CGNSSINFO formato real: fix, GPS_SVs, GLO_SVs, lat, NS, lon, EW... (SEM BDS)
- CGPSINFO funciona corretamente (formato NMEA DDMM.MMMM)

**Modificações:**
- `simcom_driver.c`: CGNSSINFO parser corrigido (removido gal_svs), mas CGNSSINFO inutilizável - passou-se a usar CGPSINFO como primário
- `simcom_driver.c`: Timeouts: CGNSSPWR 5s→9s, CGNSSINFO 5s→9s, CGPSINFO 5s→9s, COPS 5s→65s, CSQ 5s→9s, CMQTTSTART 5s→15s, CMQTTSTOP 3s→15s
- `simcom_driver.c`: `xSemaphoreTake(response_sem, 0)` para drenar semáforo
- `simcom_driver.c`: `ESP_LOGI` para AT TX (não só DEBUG)

---

## Sessão 06 — Gerenciamento Inteligente de SIM Chips
**Data:** 2026-06-16
**Objetivo:** Ler MSISDN, guardar dados SIM na NVS, troca inteligente por sinal.

**Problemas Resolvidos:**
- Números dos chips não eram lidos
- Dados SIM (IMEI, ICCID, MSISDN) não persistiam entre reboots
- Watchdog só trocava SIM quando ausente, não por sinal fraco

**Modificações:**
- `ble_mobile.h`: Novos campos e NVS keys para dados SIM
- `ble_mobile.c`: save_sim_data(), load_sim_data()
- `simcom_driver.c/h`: simcom_driver_get_msisdn() com AT+CNUM + CPBS fallback
- `simcom_driver.c`: Sonda ambos os slots, escolhe o de melhor sinal
- `simcom_driver.c`: Watchdog com sinal < -100dBm por 5 ciclos → troca
- `simcom_driver.c`: Roaming detectado via CREG stat=5
- `main.c`: Persistência SIM data na NVS no boot

---

## Sessão 07 — Correção MQTT e Encerramento do PPP
**Data:** 2026-06-16
**Objetivo:** Parar loop de erros WiFi MQTT, renomear PPP, corrigir URC.

**Problemas Resolvidos:**
- `mqtt_publisher_init()` criava `esp_mqtt_client` mesmo em CELLULAR_ONLY → erros de TCP a cada 4,5s
- URC `+CMQTTDELIVER:` não existe na documentação (corrigido para `+CMQTTRXSTART/TOPIC/PAYLOAD/END`)
- Usuário/senha não enviados no CMQTTCONNECT
- URC `+CMQTTNONET` não tratado
- Campo `ppp_connected` ainda nomeado como PPP

**Modificações:**
- `main.c`: mqtt_publisher_init só roda se não for CELLULAR_ONLY
- `main.c` dispatcher: publica direto via simcom_driver_mqtt_publish() em CELLULAR_ONLY
- `simcom_driver.c`: URC handler substituído por máquina de estado CMQTTRX*
- `simcom_driver.c`: +CMQTTNONET tratado (marca desconexão)
- `simcom_driver.c`: CMQTTCONNECT agora inclui username e password
- `simcom_driver.c`: Delay 1s entre subscribes (evita erro 14 "client busy")
- `ble_mobile.h`: ppp_connected → cellular_connected
- `ble_mobile.c`: JSON "ppp" → "cell_mqtt"
- `sdkconfig.defaults`: Configs PPP mantidas mas não usadas

---

## Sessão 08 — Correção FINAL: Config .env + GPS Parser
**Data:** 2026-06-16
**Objetivo:** Executar generate_config.py, corrigir parser GPS, offline cache.

**Problemas Resolvidos:**
- `generate_config.py` nunca executado → NET_MODE=3 (auto) ao invés de 1
- Tópicos MQTT com placeholder `000000000000` ao invés do MAC real
- Parser CGNSSINFO quebrado para chip ASR1601 (3 campos SV, não 4)
- CGPSINFO era fallback, agora é primário (funciona)
- Offline cache chamava mqtt_publisher_enqueue() em CELLULAR_ONLY
- `uart_flush_input` causava deadlock com RX task

**Modificações:**
- Executado `generate_config.py` → NET_MODE=1
- `main.c`: Substitui placeholder `000000000000` pelo MAC real nos tópicos
- `simcom_driver.c`: CGPSINFO como primário, CGNSSINFO como fallback
- `simcom_driver.c`: Removeu `uart_flush_input` (deadlock)
- `offline_cache.c`: Usa `simcom_driver_mqtt_publish()` se celular conectado
- `simcom_driver.c`: Aumentou cmd[256]→cmd[512] (warning truncation)
- Timeouts AT init: 2s→5s, CFUN=1,1:5s→10s, delay 3s→5s

---

## Sessão 09 — Documentação Final do Projeto
**Data:** 2026-06-16
**Objetivo:** Atualizar PROJETO_BASTAO.md, COMPILATION_GUIDE.md, AGENTS.md, README.md.

**Entregas:**
- `PROJETO_BASTAO.md`: Arquitetura completa, pinagens, comandos AT, fluxo de dados
- `COMPILATION_GUIDE.md`: Instruções de build, configuração, troubleshooting
- `AGENTS.md`: Diário de desenvolvimento (esta sessão)
- `README.md`: Visão geral do projeto

---

## Sessão 10 — Correção MQTT Cleanup + STM32 Wake
**Data:** 2026-06-16
**Objetivo:** Corrigir MQTT connect que travava (CMQTTACCQ ERROR) e STM32 que não acordava do sleep.

**Problemas Resolvidos:**
- `AT+CMQTTACCQ` retornava ERROR porque cliente MQTT estava preso de sessão anterior
- STM32 entrava em STOP mode e nunca mais acordava (bug na flag `stm32_sleeping`)
- Offline cache publicava em tópico hardcoded sem MAC

**Modificações:**
- `simcom_driver.c`: MQTT connect agora faz CMQTTREL + CMQTTSTOP de cleanup antes de CMQTTSTART
- `simcom_driver.c`: Retorno de CMQTTSTART agora é verificado (antes ignorado)
- `main.c`: Watchdog STM32 agora envia `\n` a cada 30s quando `stm32_sleeping=true`
- `offline_cache.c`: Tópico corrigido de hardcoded para `bastao_network_config.mqtt.topic_telemetry`

---

## Sessão 11 — Timezone: GT-4 e quarter-hours
**Data:** 2026-06-16
**Objetivo:** Corrigir horário que mostrava 16h adiantado.

**Problemas Resolvidos:**
- Torre retorna `-16` (quarter-hours = 4h), código tratava como 16 horas
- SNTP não funciona sem interface IP no ESP32 na arquitetura AT commands

**Modificações:**
- `simcom_driver.c`: `tz_h * 3600` → `tz_h * 900` (quarter-hours)
- `simcom_driver.c`: Detecção automática (tz_h > 14 → quarter-hours, senão → horas)
- `main.c`: Removido SNTP (`#include "esp_sntp.h"`, callback, init) — não funcional
- `main.c`: Tower sync como fonte primária, chamado a cada 30s enquanto clock < 2023

---

## Sessão 12 — Documentação Final do Projeto
**Data:** 2026-06-16
**Objetivo:** Atualizar documentação com todas as correções das sessões 10 e 11.

**Entregas:**
- `PROJETO_BASTAO.md`: Seções de time sync, STM32 sleep/wake, cache offline adicionadas
- `AGENTS.md`: Sessões 10-12 adicionadas
- `README.md`: Atualizado

---

## Sessão 13 — Correção Inicialização BLE Mesh (Model Binding)
**Data:** 2026-06-17
**Objetivo:** Corrigir erro de "Model not bound to AppKey 0x0000" (err=-22) no Coordenador e "GATT_SendServiceChangeIndication" na K10.

**Problemas Resolvidos:**
- O stack do BLE Mesh na ESP-IDF é assíncrono. O Coordenador tentava adicionar a AppKey e fazer o bind do modelo local sincronamente, antes da stack confirmar a adição da chave.
- O endereço do elemento primário estava hardcoded (`0x0002`), o que poderia causar falhas no bind.
- O Node K10 e o Coordenador emitiam erro sobre `GATT_SendServiceChangeIndication can't send service change indication manually`.

**Modificações:**
- `esp32_firmware/main/mesh_coordinator.c`: Refatoração da inicialização do provisionador (`mesh_provisioner_init`). Removidas as chamadas síncronas de `add_local_app_key` e `bind_app_key_to_local_model`. Agora elas são acionadas em cascata por eventos (`PROV_ENABLE_COMP_EVT` -> `ADD_LOCAL_APP_KEY_COMP_EVT` -> `BIND_APP_KEY_TO_MODEL_COMP_EVT`).
- `esp32_firmware/main/mesh_coordinator.c`: Substituído endereço hardcoded por `esp_ble_mesh_get_primary_element_address()` durante o bind.
- `k10_firmware/sdkconfig.defaults` e `esp32_firmware/sdkconfig.defaults`: Adicionado `CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MANUAL=y` para corrigir o aviso do GATT.

---

## Sessão 14 — Correção BLE Mesh: Race Condition + Hello World Test
**Data:** 2026-06-17
**Objetivo:** Corrigir comunicação entre Coordenador e K10.

**Problemas Resolvidos:**
- `k10_provisioned = true` era setado antes do AppKey ser vinculado ao modelo do K10
- Coordenador não tinha flag `local_model_bound` para rastrear bind do modelo local
- K10 não detectava provisionamento existente no NVS (reboots)
- K10 faltava `CONFIG_BLE_MESH_SETTINGS=y` para persistir subnet em RAM

**Modificações:**
- `esp32_firmware/main/mesh_coordinator.c`: Flag `local_model_bound`, `k10_provisioned` movido para `MODEL_APP_BIND`, retry em `APP_KEY_ADD`, logging detalhado
- `k10_firmware/components/k10_mesh/k10_mesh_node.c`: `esp_ble_mesh_node_is_provisioned()` para detectar NVS, logging detalhado
- `k10_firmware/sdkconfig.defaults`: `CONFIG_BLE_MESH_SETTINGS=y`
- `esp32_firmware/main/mesh_coordinator.c`: Nova função `mesh_coordinator_send_hello_test()` para teste

---

## Sessão 15 — CID BLE Mesh: 0xFFFF Bug + Encryption Bypass
**Data:** 2026-06-17
**Objetivo:** Corrigir bind do modelo vendor que falhava silenciosamente e integracao de dados reais entre Coordenador e K10.

### Problemas Resolvidos

**Bug 1 — CID=0xFFFF conflita com CID_NVAL no ESP-IDF:**
- A funcao `bt_mesh_provisioner_bind_local_model_app_idx()` do ESP-IDF v5.4 usa `CID_NVAL = 0xFFFF` como flag para indicar modelo SIG. Nosso vendor model usava `CID=0xFFFF`, fazendo a funcao procurar um modelo SIG (ID=0x0001) em vez do vendor model.
- Resultado: `bind_api retornou: 0` mas o modelo nao era vinculado — bind falso-positivo.
- K10 nunca recebia dados porque o coordenador nunca conseguia enviar (bind nao funcionava).

**Bug 2 — Encryptacao desnecessaria no caminho BLE Mesh:**
- `dispatcher_task` encriptava payload com `secure_payload_encrypt()` antes de enviar via BLE Mesh.
- K10 nao tem capacidade de descriptografar AES-256-CBC — esperava JSON simples.
- Resultado: K10 recebia string hex encriptada, procurava `\"type\":\"rfid\"` com `strstr()` e nao encontrava — payload ignorado.

### Licoes Aprendidas
- **CID 0xFFFF e reservado internamente pelo ESP-IDF BLE Mesh.** Para vendor models, usar CID registrado na Bluetooth SIG (ex: 0x02A5 = Espressif).
- **BLE Mesh link layer ja tem seguranca propria** (criptografia por AppKey + NetKey). Nao e necessario AES-256-CBC adicional entre coordenador e K10. A criptografia AES e exclusiva para o path MQTT (nuvem).
- O K10 sempre esperou JSON simples — o bug estava no coordenador que enviava dados encriptados.
- `mesh_coordinator_send_data()` ja tem guardas (`k10_provisioned && local_model_bound`) que impedem envio antes do provisionamento completo — correto.

### Modificacoes

**`esp32_firmware/main/mesh_coordinator.c`:**
- `#define CID 0xFFFF` → `#define CID 0x02A5` (Evita conflito com CID_NVAL)
- Adicionado comentario explicando porque 0x02A5 e nao 0xFFFF

**`k10_firmware/components/k10_mesh/k10_mesh_node.c`:**
- `#define CID 0xFFFF` → `#define CID 0x02A5` (Consistente com coordenador)

**`esp32_firmware/main/main.c`:**
- `dispatcher_task`: Removeu chamada a `secure_payload_encrypt()` para dados BLE Mesh
- Agora envia JSON simples diretamente via `mesh_coordinator_send_data(json_buf)`
- Criptografia AES-256-CBC mantida exclusivamente para o path MQTT (nuvem)
- Comentario documentando a decisao: "K10 nao tem capacidade de descriptografia AES-256-CBC"

### Logs de Validacao
```
I (6935) K10_MESH: MODEL_OPERATION: opcode=0xc00001 addr=0x0002 len=16
I (6936) K10_MESH: Recebido do coordenador: HELLO_WORLD_TEST
I (14952) MESH_COORDINATOR: HELLO_WORLD enviado para 0x0005: ret=0
```

---

## Sessão 16 — GPS + Celular na Tela K10 + Remoção Hello World
**Data:** 2026-06-17
**Objetivo:** Enviar coordenadas GPS e status da rede celular (RSSI, conectividade, operadora) do Coordenador para a Tela K10 via BLE Mesh.

### Problemas Resolvidos
- Tela K10 mostrava coordenadas GPS hardcoded `-20.4442, -54.6194` — agora recebe dados reais do modem SIMCom
- K10 não tinha informação sobre conectividade celular (4G online/offline)
- Hello world test estava ocupando tráfego da mesh desnecessariamente (a cada 5s)

### Modificações

**ESP32 Coordinator (`esp32_firmware/main/main.c`):**
- Removido `hello_timer` + `mesh_coordinator_send_hello_test()` do `dispatcher_task`
- Adicionado bloco de envio periódico GPS + Celular no `system_orchestrator_task` (a cada 30s):
  - GPS JSON: `{"type":"gps","lat":...,"lon":...,"fix":...,"alt":...,"speed":...}`
  - Cell JSON (a cada 60s): `{"type":"cell","rssi":...,"connected":...,"operator":"..."}`

**ESP32 Coordinator (`esp32_firmware/main/mesh_coordinator.c`):**
- Removida função `mesh_coordinator_send_hello_test()`

**ESP32 Coordinator (`esp32_firmware/main/mesh_coordinator.h`):**
- Removido protótipo `mesh_coordinator_send_hello_test()`

**K10 (`k10_firmware/components/k10_mesh/k10_mesh_node.h`):**
- Adicionadas structs `k10_gps_data_t` (lat, lon, fix, alt, speed) e `k10_cell_status_t` (rssi, connected, operator_name)
- Adicionados getters `k10_mesh_get_gps_data()` e `k10_mesh_get_cell_status()`

**K10 (`k10_firmware/components/k10_mesh/k10_mesh_node.c`):**
- Adicionado parsing de `"type":"gps"` e `"type":"cell"` em `k10_mesh_process_payload()`
- Adicionadas variáveis estáticas e implementações dos getters

**K10 (`k10_firmware/components/gui/include/gui_manager.h`):**
- Adicionados protótipos `gui_manager_update_gps()` e `gui_manager_update_cell_status()`

**K10 (`k10_firmware/components/gui/gui_manager.c`):**
- `icon_4g` movido de variável local para static global (para permitir atualização de cor)
- `gui_manager_update_gps()`: atualiza `lbl_gps_coords` com coordenadas reais (verde) ou "GPS: SEM SINAL" (vermelho)
- `gui_manager_update_cell_status()`: ícone 4G fica verde (conectado) ou vermelho (desconectado)

**K10 (`k10_firmware/main/main.c`):**
- Adicionado polling de `k10_gps_data_t` e `k10_cell_status_t` no `gui_task` (a cada 500ms)

### Fluxo de Dados
```
Coordinator (system_orchestrator_task):
  GPS a cada 30s → mesh_coordinator_send_data(json_gps)
  Cell a cada 60s → mesh_coordinator_send_data(json_cell)
        ↓
K10 (k10_mesh_process_payload):
  strstr("gps") → preenche k10_gps_data_t (valid=1)
  strstr("cell") → preenche k10_cell_status_t (valid=1)
        ↓
K10 (gui_task, a cada 500ms):
  gps->valid → gui_manager_update_gps() → lbl_gps_coords atualizado
  cell->valid → gui_manager_update_cell_status() → icon_4g cor atualizada
```

---

## Sessão 17 — Otimização SIMCom (Captura de Tags) + GPS Hot Start
**Data:** 2026-06-18
**Objetivo:** Resolver travamento na captura de tags RFID decorrente de contenção no mutex do modem SIMCom 7663E e habilitar Hot Start no receptor GPS.

### Problemas Resolvidos
- **Perda de Tags no Modo CELLULAR_ONLY:** O processamento serial de tags no `dispatcher_task` era síncrono e concorria diretamente com comandos AT longos do modem, causando estouro de buffers e perda de tags. Agora a fila assíncrona e a task do publicador MQTT rodam incondicionalmente, desacoplando o fluxo.
- **Deadlocks/Estouro de Tasks no Watchdog:** O watchdog recriava a task `simcom_rx` e deletava o driver UART repetidamente nas reconexões, gerando conflitos. Agora a task e o driver são instanciados uma única vez.
- **Sondagem de Chip Lenta:** Re-inicializações do modem sofriam com 15 segundos de atraso sondando ambos os slots (DSSS). Implementada flag `first_init_done` para pular sondagem redundante e ir direto para o chip ativo.
- **Mutex Lock por Leitura de Rede Celular:** A leitura periódica da operadora e sinal executava comandos AT bloqueantes de até 65s no orchestrator. Agora `simcom_driver_get_status` consome métricas puramente em cache, sendo atualizada de forma lenta (background de 5 min) no watchdog.
- **GPS Hot Start via AP_Flash:** Mudado o trigger de energia do GPS para salvar orbit data na Flash (`AT+CGNSSPWR=0,1`) e carregá-la no boot (`AT+CGNSSPWR=1,1`), reduzindo drasticamente o TTFF (Time to First Fix).

### Modificações

**MQTT Publisher (`esp32_firmware/main/mqtt_publisher.c`):**
- Alterado `mqtt_publisher_init()` e `update_config` para sempre instanciar a fila e a task, omitindo o cliente `esp_mqtt` nativo apenas se estiver em `CELLULAR_ONLY`.

**ESP32 Coordinator (`esp32_firmware/main/main.c`):**
- Simplificada a `dispatcher_task` para sempre encaminhar mensagens à fila via `mqtt_publisher_enqueue`.
- `app_main()` sempre inicializa o publicador MQTT.
- `system_orchestrator_task` atualiza qualidade de sinal celular lendo diretamente o RSSI em cache em `bastao_current_status` em vez de chamar comandos AT ativos.

**SIMCom Driver (`esp32_firmware/main/simcom_driver.c`):**
- Adicionadas variáveis `simcom_rx_task_handle` e `first_init_done`.
- `simcom_driver_init()` impede a recriação da task UART e deleção do driver se já configurados.
- `at_init_sequence()` ignora varredura física de slots e seleciona diretamente o slot ativo se `first_init_done` for true.
- `simcom_driver_gps_power_on()` configurado com `AT+CGNSSPWR=1,1` e `simcom_driver_gps_power_off()` com `AT+CGNSSPWR=0,1`.
- `simcom_driver_get_status()` otimizado para expor apenas métricas do cache.
- `simcom_watchdog_task` atualiza informações pesadas (`simcom_driver_get_network_info` e `query_ceer_log`) de forma lenta a cada 5 minutos em background.

---

## Sessão 18 — Correção de Reconexão BLE Mesh (Persistência NVS no Coordenador)
**Data:** 2026-06-18
**Objetivo:** Resolver falha de comunicação após reboots/gravações decorrente de perda de estado do provisionador no Coordenador.

**Problemas Resolvidos:**
- O Coordenador (Provisioner) perdia o estado C e a tabela de nós provisionados após reiniciar por falta de persistência NVS (`CONFIG_BLE_MESH_SETTINGS`). O K10 (Node) restaurava seu estado do NVS e não enviava beacons não-provisionados. Como resultado, o Coordenador ficava esperando beacons indefinidamente e falhava ao enviar dados via BLE Mesh (retornando `-1`).
- Habilitada a persistência de configurações no Coordenador.
- Implementado escaneamento automático do banco de dados do provisionador no boot para restaurar o endereço e estado de provisionamento do K10 (`k10_addr` e `k10_provisioned`).

**Modificações:**
- `esp32_firmware/sdkconfig.defaults` e `esp32_firmware/sdkconfig`: Adicionado `CONFIG_BLE_MESH_SETTINGS=y` e configurações de timeout/taxa de persistência associadas.
- `esp32_firmware/main/mesh_coordinator.c`: Na inicialização (`ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT`), se o bind do modelo local for concluído com sucesso, o banco de dados de nós do provisionador é consultado para restaurar o endereço do K10 se ele já estiver provisionado.

---

## Sessão 19 — Otimização de Mensagens BLE Mesh + Correção de Alerta de Bateria
**Data:** 2026-06-18
**Objetivo:** Resolver congestionamento na rede BLE Mesh da K10 (segment exhaustion) e loop infinito de buzzer longo por leitura de bateria.

**Problemas Resolvidos:**
- **Battery Buzzer Loop:** O threshold de bateria baixa em `main.c` do ESP32 estava hardcoded em `< 9.0V`. Como a bateria opera na faixa de 7.5V a 8.9V, o alarme de bateria baixa disparava continuamente a cada 5s mesmo com carga cheia (ex: 8.85V). Fator de trigger corrigido para `< 7.7V` (~14% de bateria).
- **BLE Mesh Congestion:** A K10 transmitia dados de acelerômetro (`OP_ACCEL`) a cada 500ms incondicionalmente, estourando o buffer de segmentos da pilha BLE Mesh (`No multi-segment message contexts available`). O código foi otimizado para enviar dados apenas sob mudança de movimento ou a cada 10s em caso de heartbeat estático.
- **Capacidade de Segmentos BLE Mesh:** Aumentada a capacidade de contextos de segmentos concorrentes (`CONFIG_BLE_MESH_TX_SEG_MSG_COUNT` e `CONFIG_BLE_MESH_RX_SEG_MSG_COUNT`) de 1 para 4 em ambos os projetos (`esp32_firmware` e `k10_firmware`).

**Modificações:**
- `esp32_firmware/main/main.c`: Threshold de bateria baixa alterado de `9.0f` para `7.7f`.
- `k10_firmware/main/main.c`: Envio de acelerômetro condicionado à presença de movimento ou a cada 10s.
- `esp32_firmware` e `k10_firmware` (`sdkconfig` / `sdkconfig.defaults`): Segment counts alterados de 1 para 4.

---

## Sessão 20 — Tela K10: GPS e Bateria + Envio Imediato de Sensores
**Data:** 2026-06-18
**Objetivo:** Ajustar exibição de coordenadas GPS na tela, alterar fonte da telemetria de bateria de STM32 para K10, e enviar sensores da K10 imediatamente ao ler tag para atualizar o MQTT.

**Problemas Resolvidos:**
- Exibição de coordenadas GPS na tela K10 mostrava valores falsos ou estáticos no boot e "GPS: SEM SINAL" sob falta de fix. Agora inicializa com "GPS NÃO SINCRONIZADO" em vermelho e exibe o mesmo quando perde o sinal.
- Bateria do bastão (STM32) spammava buzzer e telemetria de 8.85V que era convertida de forma errada. Agora a leitura de bateria do STM32 é ignorada no ESP32 e silenciada, e a bateria da tela K10 (0-100%) passa a ser a fonte de telemetria de bateria para o payload MQTT.
- Telemetria de acelerômetro e bateria enviadas ao MQTT no momento de leitura da tag estavam desatualizadas. Agora, ao receber a tag RFID via BLE Mesh, o nó K10 imediatamente lê o acelerômetro e bateria locais e envia ao Coordinator via Mesh. O Coordinator aguarda 300ms antes de gerar o payload MQTT para garantir o recebimento desses sensores atualizados.

**Modificações:**
- `k10_firmware/components/gui/gui_manager.c`: Inicializa `lbl_gps_coords` como "GPS NÃO SINCRONIZADO" (em vermelho) e altera no-fix de "GPS: SEM SINAL" para "GPS NÃO SINCRONIZADO".
- `k10_firmware/main/main.c`: No loop de `rfid->valid`, realiza leituras imediatas de `hal_sensors_read_accel` e `hal_sensors_read_battery` e as envia na hora com `k10_mesh_send_accel` e `k10_mesh_send_display_status`.
- `esp32_firmware/main/mesh_coordinator.c`: Sincroniza `bastao_current_status.battery_voltage` com `volt` em `OP_DISPLAY_STATUS`.
- `esp32_firmware/main/main.c`: Ignora pacotes UART de bateria do STM32 (`DATA_TYPE_BATTERY`).
- `esp32_firmware/main/main.c`: No despachante de tags, envia a tag para a Mesh, executa um delay de 300ms, formata o JSON MQTT coletando `bastao_current_status.k10_battery_percentage` e os dados de acelerômetro mais recentes, e envia para a nuvem.

---

## Sessão 21 — Ícones da Tela K10 Condicionais por Sinal e Modo
**Data:** 2026-06-18
**Objetivo:** Otimizar e condicionar a exibição de ícones (RFID, 4G, WiFi) no cabeçalho da K10 com base nos status reais de conectividade, potência de antena e modo de rede.

**Problemas Resolvidos:**
- Ícones de status iniciavam estáticos e verdes no boot, dando falsa indicação de conexões ativas.
- Ícone RFID agora inicia em vermelho e só fica verde quando a conexão UART com o STM32 (readers online) estiver ativa (`stm32_alive`).
- Ícone 4G agora é ocultado por padrão e, ao conectar, exibe a potência real medida em dBm (ex: "4G -85dBm") em verde. Ao desconectar, oculta-se automaticamente.
- Ícone WiFi é ocultado por padrão e só é exibido quando o modo de rede ativo do coordenador for Wi-Fi ou Dual/Auto (ocultando-se no modo exclusivo celular).

**Modificações:**
- `k10_firmware/components/gui/gui_manager.c`: Declarou `icon_wifi`, `icon_rfid` e `icon_gps` estáticos globals. Inicializa ícones ocultados ou em vermelho no boot.
- `k10_firmware/components/gui/gui_manager.c`: Atualiza `gui_manager_update_cell_status` para renderizar o RSSI (ex: "4G -85dBm") e gerenciar visibilidade do 4G. Implementa `gui_manager_update_gateway_status` para gerenciar cores do RFID e visibilidade do WiFi.
- `k10_firmware/components/k10_mesh/k10_mesh_node.c` / `k10_mesh_node.h`: Implementa parse do tipo de pacote `"status"` e getter `k10_mesh_get_gateway_status()`.
- `k10_firmware/main/main.c`: Processa o novo status na task de GUI.
- `esp32_firmware/main/main.c`: Envia o payload `{"type":"status","rfid_conn":%d,"wifi_active":%d}` a cada 30 segundos via Mesh.

---

## Sessão 22 — Correção de Travamento por Sleep e Controle de Backlight na Tela K10
**Data:** 2026-06-22
**Objetivo:** Solucionar o travamento da UART (RFID/SIMCom) e BLE Mesh ao entrar em light sleep no ESP32 Coordenador, implementando em contrapartida um desligamento de backlight inteligente e temporizado na tela K10 com acionamento por botões ou RFID.

**Problemas Resolvidos:**
- **Coordenador Freeze**: A suspensão de clock e barramentos no light sleep interrompia as comunicações com o modem e módulo RFID, travando o firmware do Coordenador. O light sleep foi desabilitado no ESP32.
- **Backlight Timeout na Tela**: Implementado timer de inatividade de 60s na K10 para desligar o backlight da tela via barramento I2C do expansor XL9535 (`xl9535_write_reg(0x02, 0x00)`).
- **Acionamento por Botões (Wakeup Suave)**: Interceptados pressionamentos dos botões (Key A / Key B) no driver de input da K10. Se a tela estiver apagada, ela acorda e ignora a primeira tecla pressionada para evitar ações indesejadas no painel.
- **Acionamento por RFID**: Ao receber uma tag válida do coordenador via Mesh, a K10 reativa imediatamente o backlight da tela.

**Modificações:**
- `esp32_firmware/main/esp_power.c`: Desabilitado `esp_power_enter_light_sleep()`.
- `k10_firmware/components/k10_hal/include/hal_display.h`: Declarado `hal_display_set_backlight`, `hal_display_reset_inactivity` e `hal_display_check_timeout`.
- `k10_firmware/components/k10_hal/hal_display.c`: Implementadas as funções de controle de backlight e checagem de timeout/atividade.
- `k10_firmware/components/k10_hal/hal_buttons.c`: Alterado `keypad_read` para acordar a tela e ignorar a tecla pressionada no primeiro ciclo de clique.
- `k10_firmware/main/main.c`: Chamado `hal_display_reset_inactivity()` no processamento de RFID e `hal_display_check_timeout()` no loop de GUI.

---

## Sessão 23 — Calibração e Envio da Bateria do STM32 (Bastão)
**Data:** 2026-06-22
**Objetivo:** Configurar e enviar a porcentagem da bateria principal do bastão (STM32) para a nuvem via MQTT e disponibilizar os dados na rede local via BLE Mesh.

**Problemas Resolvidos:**
- Mapeado o range analógico da bateria do STM32 de 8.80V (0%) a 8.89V (100%).
- Reabilitado o processamento de telemetria `DATA_TYPE_BATTERY` no despachante do ESP32 Coordenador.
- O payload MQTT agora prioriza o percentual calculado da bateria do bastão (STM32) como `nivel_bateria` e usa a bateria da K10 como fallback.
- Enviados os dados de bateria do STM32 para o nó K10 via Mesh para logging/diagnóstico.

**Modificações:**
- `esp32_firmware/main/main.c`: Habilitada recepção de `DATA_TYPE_BATTERY`. Adicionado mapeamento de tensão para porcentagem (8.80V a 8.89V). Atualizado despachante MQTT para priorizar bateria do bastão.
- `k10_firmware/components/k10_mesh/k10_mesh_node.c`: Atualizada a máquina de estados que processa pacotes `"type":"batt"` para também decodificar o percentual (`pct`) recebido.
- `k10_firmware/components/gui/gui_manager.c`: Modificada a exibição do ícone 4G no cabeçalho para exibir barras de sinal (`||||` a `|`) mapeadas a partir do RSSI (dBm) ao invés do valor numérico bruto.

---

## Sessão 24 — Exibição Gráfica da Bateria do Bastão na K10
**Data:** 2026-06-23
**Objetivo:** Corrigir a exibição gráfica da bateria do Bastão (recebida via Mesh) no cabeçalho da tela K10.

**Problemas Resolvidos:**
- O ícone de bateria no cabeçalho da K10 exibia 0% e cor vermelha permanentemente, ignorando os pacotes do coordenador.
- `gui_manager_update_battery_mesh` não atualizava os componentes gráficos (`lbl_bat_pct` e `icon_bat_header`), apenas exibia log.
- O loop de sensores locais a cada 500ms sobrescrevia o indicador de bateria no cabeçalho com a leitura da bateria local da tela K10 (0% por estar sem bateria / USB).

**Modificações:**
- `k10_firmware/components/gui/gui_manager.c`:
  - Implementada lógica de atualização de componentes LVGL (`lbl_bat_pct` e `icon_bat_header`) na recepção da telemetria Mesh em `gui_manager_update_battery_mesh()`.
  - Removido o overwrite do cabeçalho pela bateria local em `gui_manager_update_sensors()`.

---

## Sessão 25 — Correção na Formatação de Tag RFID WL-134 (newlib-nano %llu Bug)
**Data:** 2026-06-23
**Objetivo:** Corrigir a formatação do código do brinco (RFID WL-134) que vinha incorreto e truncado como `90000000000000lu`.

**Problemas Resolvidos:**
- O formato de tag FDX-B continha a string `90000000000000lu` devido à limitação da biblioteca padrão `newlib-nano` usada no STM32, que não possui suporte habilitado para inteiros de 64 bits (`long long`) via `%llu` no `sprintf`. O compilador interpretava `%llu` como `%lu` (exibindo os 32 bits superiores/inferiores zerados) seguidos de um sufixo literal `"lu"`.
- Implementada a função auxiliar `format_uint64_padded` para conversão e formatação direta de `uint64_t` sem depender do suporte libc.

**Modificações:**
- `stm32_firmware/Core/Src/rfid_parser.c`:
  - Adicionado helper `format_uint64_padded()`.
  - Atualizada a geração do JSON RFID em `RFID_Process_WL134()` para usar a string convertida manualmente.

---

## Sessão 26 — Filtro e Média da Leitura de Bateria STM32
**Data:** 2026-06-23
**Objetivo:** Aumentar a precisão e estabilidade da leitura de tensão da bateria principal no STM32 eliminando ruídos.

**Problemas Resolvidos:**
- A leitura de tensão sofria de pequenas variações instantâneas (ruídos de ADC).
- Alterada a quantidade de amostras da média móvel de `8` para `10` em `battery_monitor.h`.
- Modificada a função `Battery_Read()` em `battery_monitor.c` para colher 10 amostras consecutivas do ADC e tirar a média delas antes de inserir o valor na fila da média móvel.

**Modificações:**
- `stm32_firmware/Core/Inc/battery_monitor.h`: Aumentado `BATTERY_SAMPLES` de 8 para 10.
- `stm32_firmware/Core/Src/battery_monitor.c`: Modificada `Battery_Read()` para tirar a média de 10 leituras consecutivas do ADC.

---

## Sessão 27 — Remoção Completa de Sleep (STM32 + ESP32)
**Data:** 2026-06-24
**Objetivo:** Eliminar todos os modos de baixo consumo (sleep/stop) do STM32 e ESP32 para garantir operação contínua.

**Problemas Resolvidos:**
- STM32 entrava em STOP1 mode após 120s de inatividade, desligando leitores RFID e pausando processamento
- ESP32 light sleep causava travamento nas comunicações UART (RFID/SIMCom) — já havia sido desabilitado mas código permanecia
- Watchdog do ESP32 enviava `\n` periódico para acordar STM32, adicionando complexidade desnecessária
- Módulo `power_mgmt.c/h` com 159 linhas de código de sleep que nunca deveria ser usado

**Modificações:**

**STM32:**
- `Core/Inc/power_mgmt.h`: Reduzido de 31 para 12 linhas — só resta `Power_Init()` sem sleeps
- `Core/Src/power_mgmt.c`: Reduzido de 159 para 11 linhas — só liga GPIO dos RFID readers
- `Core/Inc/main.h`: Removeu declarações de `Power_Update/Sleep/Wake/IsSleeping/ActivityDetected`
- `Core/Src/main.c`: Removeu `if (!Power_IsSleeping())`, `Power_Update()`, e 3 chamadas de `Power_ActivityDetected()`

**ESP32:**
- `esp_power.h`: Reduzido de 32 para 12 linhas — só resta `esp_power_init()`
- `esp_power.c`: Reduzido de 136 para 13 linhas — init vazio (só log)
- `stm32_uart.c`: Removeu variável `stm32_sleeping`, parsing de `"type":"power"`, função `stm32_uart_is_stm32_sleeping()`
- `stm32_uart.h`: Removeu `stm32_uart_is_stm32_sleeping()`, `DATA_TYPE_POWER` mantido como reservado
- `main.c`: Removeu `#include "esp_power.h"`, `esp_power_init()`, `esp_power_update()` (2x), `esp_power_trigger_wake()`, e bloco de STM32 dormindo com wake `\n` a cada 30s

---

## Sessão 28 — Potência TX do YRM100 e Ajuste de Bateria
**Data:** 2026-06-24
**Objetivo:** Configurar potência RF do leitor YRM100 conforme configuração do sistema e corrigir mapeamento de bateria.

**Problemas Resolvidos:**
- YRM100 operava sempre na potência default do módulo (26dBm), ignorando configuração do app
- Comando `0xB6` (Set TX Power) nunca era enviado ao YRM100 — faltava implementação no STM32
- Comando `stm32_cmd_send_yrm_power()` só controlava GPIO (liga/desliga) sem ajustar RF
- Mapeamento de bateria estava incorreto: 100% em 8.89V e 0% em 8.80V (range de apenas 0.09V)
- Bateria em 8.85V (carga normal) marcava apenas ~55%

**Modificações:**

**STM32:**
- `Core/Inc/rfid_parser.h`: Adicionado `YRM100_SetTXPower(uint8_t dbm)`
- `Core/Src/rfid_parser.c`: Implementada `YRM100_SetTXPower()` — monta frame `0xB6` com potência em centésimos de dBm
- `Core/Src/alerts.c`: Parsing do comando `{"cmd":"yrm_tx_power","value":<dbm>}` recebido do ESP32

**ESP32:**
- `stm32_cmd.h/c`: Nova função `stm32_cmd_send_yrm_tx_power(uint8_t dbm)` — envia JSON com dBm
- `ble_mobile.c`: Default `.yrm100_power` alterado de 20 para 26 dBm
- `main.c`: No boot, após reset do STM32, envia comando com `bastao_current_config.yrm100_power`
- `main.c`: Ajustado mapeamento de bateria para 100%=8.80V e 0%=7.50V (2 ocorrências)

---

## Sessão 29 — Remoção de Acentos da Tela K10
**Data:** 2026-06-24
**Objetivo:** Remover caracteres acentuados dos textos exibidos no display da K10.

**Modificações:**
- `k10_firmware/components/gui/gui_manager.c`: `"GPS NÃO SINCRONIZADO"` → `"GPS NAO SINCRONIZADO"` (2 ocorrências)

---

## Sessão 30 — Correção do Makefile STM32 e Compilação
**Data:** 2026-06-24
**Objetivo:** Corrigir o path do linker script no makefile de Release para permitir compilação via linha de comando.

**Problemas Resolvidos:**
- Makefile do Release usava `Bastão-ESP` (com acento) no caminho absoluto do linker script, mas a pasta real era `Bastao-ESP` (sem acento)
- Arquivos `rfid_parser.c`, `battery_monitor.c` e `circular_buffer.c` não estavam no `objects.list` gerado pelo STM32CubeMX

**Modificações:**
- `stm32_firmware/Release/makefile`: Corrigido path do linker script de `Bastão-ESP` para `Bastao-ESP`
- `stm32_firmware/Release/objects.list`: Adicionados `rfid_parser.o`, `battery_monitor.o`, `circular_buffer.o`

---

## Sessão 31 — Otimização de Velocidade: K10 + STM32
**Data:** 2026-06-24
**Objetivo:** Reduzir latência entre leitura RFID e exibição na tela K10, e aumentar taxa de leitura do YRM100.

**Problemas Resolvidos:**

**K10 lenta para exibir tags:**
- `dispatcher_task` no ESP32 tinha `vTaskDelay(300ms)` após cada envio Mesh, esperando resposta de sensores da K10 — bloqueava processamento de tags subsequentes
- GUI da K10 fazia polling de dados Mesh a cada 500ms — tag podia demorar até 500ms para aparecer na tela

**STM32 com baixa taxa de leitura:**
- YRM100 fazia inventário a cada 200ms no mínimo (5 leituras/s)
- Com buffer cheio, esperava 500ms antes de novo poll

**Modificações:**

**ESP32:**
- `main.c`: Removido `vTaskDelay(300ms)` após `mesh_coordinator_send_data()` — K10 responde assincronamente e dados de sensores chegam via Mesh no ritmo dela

**K10:**
- `main.c`: Polling de dados Mesh reduzido de 500ms para 200ms (`sensor_timer >= 50` → `>= 20`)

**STM32:**
- `Core/Src/main.c`: Poll mínimo do YRM100 reduzido de 200ms para 100ms; poll com buffer ocupado reduzido de 500ms para 250ms

---

## Sessão 32 — Correção Criptografia Mesh + YRM100 SetTXPower Perdidos no Git HEAD
**Data:** 2026-06-24
**Objetivo:** Resolver K10 sem dados (payload criptografado) e YRM100 sem leitura (TX power não configurado) após git HEAD reset.

### Problemas Resolvidos
- **K10 não exibia dados:** `dispatcher_task` no ESP32 criptografava o JSON com `secure_payload_encrypt()` antes de enviar via BLE Mesh. O K10 recebia hex (`a1b2c3...`) e `strstr()` nunca encontrava `"type":"rfid"`. Perdido no reset do git HEAD (Sessão 15).
- **YRM100 não lia tags:** Função `YRM100_SetTXPower()` e handler do comando `yrm_tx_power` no STM32 foram perdidos no reset (Sessão 28). Modulo operava sem configuração explícita de potência RF.
- **Status periódico perdido:** GPS, Cell e Gateway status não eram mais enviados à K10 (Sessões 16 e 21).
- **Bateria sem percentual:** Mensagem `batt` não incluía `pct` que o K10 espera.

### Modificações

**ESP32 (`esp32_firmware/main/main.c`):**
- `dispatcher_task`: Remove `secure_payload_encrypt()` do path Mesh — envia `json_buf` direto para `mesh_coordinator_send_data()`
- `dispatcher_task`: Adiciona cálculo de `pct` (7.50V=0%, 8.80V=100%) na mensagem `{"type":"batt",...}`
- `system_orchestrator_task`: Envia GPS + Gateway status à K10 a cada 30s
- `system_orchestrator_task`: Envia Cell status (RSSI, operadora) à K10 a cada 60s

**STM32 (`stm32_firmware`):**
- `Core/Inc/rfid_parser.h`: `RFID_BUFFER_SIZE` 128 → 512, adicionado protótipo `YRM100_SetTXPower()`
- `Core/Src/rfid_parser.c`: Implementa `YRM100_SetTXPower()` — comando `0xB6` com potência em centésimos de dBm
- `Core/Src/main.c`: Chama `YRM100_SetTXPower(26)` no boot, após power-on do módulo
- `Core/Src/alerts.c`: Handler `{"cmd":"yrm_tx_power","value":<dbm>}`, include `rfid_parser.h`

---

## Sessão 33 — Correção MQTT Publish (CMQTTREL ausente) + Race Condition GPS/Time
**Data:** 2026-06-25
**Objetivo:** Corrigir ciclo infinito de falha MQTT que impedia GPS e time sync de funcionar.

### Problemas Resolvidos
- `AT+CMQTTREL=0` nunca era chamado após `AT+CMQTTPUB` em `simcom_driver_mqtt_publish()`. O message ID 0 ficava ocupado, fazendo o próximo `AT+CMQTTTOPIC=0,34` retornar `+CMQTTTOPIC: 0,14` ERROR.
- Isso criava um loop: publish OK → próximo topic ERROR → cache re-enqueue → 500ms → retry → publish OK → topic ERROR...
- O tráfego MQTT contínuo saturava o mutex e a UART, impedindo comandos GPS (`AT+CGPSINFO`) e time sync (`AT+CCLK?`) de executar.
- O mutex era liberado ANTES do `CMQTTPUB`, permitindo que outra task (GPS) enviasse comandos AT no meio da transação MQTT, corrompendo os buffers de resposta (`awaiting_response` / `cmd_response_buf` / `response_sem` globais).

### Modificações
- `esp32_firmware/main/simcom_driver.c`:
  - `simcom_driver_mqtt_publish()`: Adicionado `AT+CMQTTREL=0` no início (cleanup de estado residual) e após `CMQTTPUB` bem-sucedido (libera message ID 0).
  - Mutex agora é mantido durante TODO o ciclo (topic → payload → publish → release), eliminando race condition entre MQTT e outros comandos AT.
  - `xSemaphoreGive(simcom_mutex)` movido para depois do `CMQTTREL`, não mais antes do `CMQTTPUB`.
  - `simcom_mutex` é `xSemaphoreCreateMutex()` — herança de prioridade FreeRTOS previne priority inversion quando orchestrator (prio 5) espera publish task (prio 4).

---

## Sessão 38 — Remoção de Configuração YRM100 do STM32 (SetTXPower)
**Data:** 2026-06-25
**Objetivo:** Remover todas as configurações enviadas ao módulo YRM100 via STM32. O usuário passará a configurar o YRM100 diretamente (via terminal serial), sem intervenção do STM32.

### Problemas Resolvidos
- O comando `0xB6` (Set TX Power) era enviado ao YRM100 pelo STM32 durante o boot (`YRM100_SetTXPower(26)`) e sob demanda via ESP32 (`yrm_tx_power`). Isso causava conflitos de configuração quando o usuário tentava configurar o módulo diretamente.
- A função `YRM100_SetTXPower()` e todo o pipeline de configuração foram removidos.

### Remoções

**STM32:**
- `Core/Inc/rfid_parser.h`: Removido protótipo `YRM100_SetTXPower(uint8_t dbm)`
- `Core/Src/rfid_parser.c`: Removida função `YRM100_SetTXPower()` (comando `0xB6` com potência em centésimos de dBm)
- `Core/Src/main.c`: Removida chamada `YRM100_SetTXPower(26)` no boot
- `Core/Src/alerts.c`: Removido handler do comando `{"cmd":"yrm_tx_power","value":<dbm>}`

**ESP32:**
- `main/stm32_cmd.c`: Removida função `stm32_cmd_send_yrm_tx_power()`
- `main/stm32_cmd.h`: Removido protótipo `stm32_cmd_send_yrm_tx_power()`
- `main/main.c`: Removida chamada `stm32_cmd_send_yrm_tx_power(bastao_current_config.yrm100_power)` no boot

### Mantido
- Alimentação do YRM100 via GPIO (`yrm_power`, `rfid_on`/`rfid_off`) — essencial para ligar/desligar o módulo
- RX UART + buffer circular + parsing de frames (`RFID_Process_YRM100`) — necessário para receber tags
- Comando de inventário `0x22` (poll a cada 100-250ms) — operação, não configuração
- Campo `yrm100_power` na config BLE — armazenado mas não enviado ao módulo

---

# Guia de Referência: Agents e Skills do Projeto Bastao-ESP

## Agents Disponíveis

| Agent | Quando Usar | Automação |
|-------|-------------|-----------|
| `explore` | Busca rápida de arquivos, grep, glob patterns | Automático |
| `general` | Tarefas multi-step, pesquisa complexa | Automático |
| `esp32-firmware-agent` | Código firmware ESP32 (ESP-IDF v5.4) | Manual |
| `k10-firmware-agent` | Firmware da Tela K10 (LVGL + BLE Mesh Node) | Manual |
| `stm32-firmware-agent` | Firmware STM32 (RFID, ADC, UART) | Manual |
| `sistema-bastao-agent` | Backend Python (FastAPI, MQTT, PostgreSQL) | Manual |
| `frontend-mobile-agent` | Frontend React SPA + App Mobile Expo | Manual |
| `testing-agent` | Testes automatizados (Python, pytest) | Manual |
| `reviewer-agent` | Code review e verificação de padrões | Manual |
| `documenter` | Geração de documentação (Doxygen, README) | Manual |
| `architect-agent` | Arquitetura de sistema, decisões de design | Manual |

### Como Chamar Agents Manuais

```
# Usando o task tool com subagent_type:
task(description="...", prompt="...", subagent_type="esp32-firmware-agent")
```

**Regra:** Agents marcados como "Manual" só devem ser chamados quando o usuário solicita explicitamente.

---

## Skills Disponíveis

| Skill | Escopo | Arquivos |
|-------|--------|----------|
| `c-best-practices` | Padrões C (Doxygen, naming, segurança) | Todos os `.c`/`.h` |
| `esp32-connectivity` | BLE Mesh, UART, MQTT, OTA, Cache | `esp32_firmware/` |
| `esp32-idf-agent` | Inicialização ESP-IDF, NVS, FreeRTOS | `esp32_firmware/` |
| `k10-firmware` | LVGL, display, acelerometro, mesh node | `k10_firmware/` |
| `stm32-firmware` | RFID, ADC bateria, UART, non-blocking | `stm32_firmware/` |
| `security-crypto` | AES-256-CBC, mbedTLS, PKCS#7 | `secure_payload.*` |
| `sistema-backend` | FastAPI, MQTT receptor, Docker | Backend Python |
| `frontend-react` | React SPA, Expo, RBAC, Leaflet | Frontend |
| `testing` | Scripts Python, pytest, CI/CD | `teste_automatizado/` |
| `project-management` | ROADMAP, progress.md, AGENTS.md | Docs |
| `simcom-7663e` | AT commands, GPS, PPP | `simcom_driver.*` |

### Como Usar Skills

```
# Skill é injetada automaticamente quando o contexto é relevante.
# Ou manualmente:
skill(name="c-best-practices")
```

---

## Padrões de Código por Contexto

### Quando编码ar código ESP32:
1. Ler skill `esp32-connectivity` e `esp32-idf-agent`
2. Seguir ordem de inicialização: NVS → Netif → Event → Módulos
3. Usar `esp_err_t` para retorno de erros
4. Header guards obrigatórios
5. Doxygen em toda função pública

### Quando编码ar código K10:
1. Ler skill `k10-firmware`
2. Tasks: `gui_task` Core 1, `mesh_node_task` Core 0
3. LVGL: `lv_tick_inc()` + `lv_timer_handler()` no loop
4. Não bloquear task LVGL com I/O

### Quando编码ar código STM32:
1. Ler skill `stm32-firmware`
2. Buffer circular para RFID
3. Non-blocking: timeouts em polling
4. ADC com `HAL_ADC_Start` + DMA

### Quando编码ar criptografia:
1. Ler skill `security-crypto`
2. AES-256-CBC com mbedTLS
3. PKCS#7 padding
4. Nunca logar chaves em texto plano

### Quando编码ar testes:
1. Ler skill `testing`
2. Scripts Python com `pytest`
3. Retornar exit code 0/1
4. Output formatado para CI

---

## Fluxo de Trabalho Recomendado

```
1. Entender o problema → skill mais relevante
2. Explorar código → agent `explore` ou `task(subagent_type="explore")`
3. Implementar → seguir padrões da skill
4. Verificar → lint, build, testes
5. Documentar → AGENTS.md (sessão) + comments Doxygen
```

---

---

## Sessão 34 — Otimização Completa da Pilha FreeRTOS
**Data:** 2026-06-25
**Objetivo:** Reorganizar prioridades, eliminar contenção de mutex para garantir RFID no topo, GPS/time non-blocking, datalogger offline estável.

### Problemas Raiz

1. **3 tasks competindo pelo `simcom_mutex`**: `mqtt_pub_task` + `cache_sync_task` + `system_orchestrator` — todas acessavam o driver SIMCom diretamente.
2. **Cache sync bypassava a fila MQTT**: `offline_cache_sync_task` chamava `simcom_driver_mqtt_publish()` direto quando celular conectado, sem passar pela `mqtt_publish_queue`.
3. **GPS e time sync bloqueantes**: `system_orchestrator` chamava `simcom_driver_get_gps()` e `simcom_driver_sync_time_from_tower()` com `at_send_cmd()` bloqueante (mutex timeout 4s). Com MQTT publicando por 3-10s, GPS e time sempre timeoutavam.
4. **RX tasks em prioridade baixa**: podiam perder bytes UART.
5. **`AT+CMQTTREL=0` ausente**: message ID 0 nunca liberado após publish (Sessão 33).

### Modificações

**`offline_cache.c`**: Cache sync agora sempre enfileira via `mqtt_publisher_enqueue()` — nunca mais chama `simcom_driver_mqtt_publish()` diretamente. Removeu dependência de `simcom_driver.h`. Elimina 1 concorrente do mutex.

**`main.c`**: 
- `offline_cache_sync_task_start(3)` → `(2)` — cache não é prioritário
- `stm32_uart_rx_task_start(5)` → `(6)` — RX no topo do Core 0
- GPS e time sync agora checam `simcom_driver_is_busy()` antes de chamar — se modem ocupado, pulam o ciclo (non-blocking)
- RSSI celular lido do cache (`simcom_driver_get_cached_rssi()`) sem AT command

**`simcom_driver.c`**:
- `simcom_rx` priority 5 → **6** (topo do Core 1)
- Adicionado `simcom_driver_get_cached_rssi()` — retorna RSSI sem tocar no modem

**`mqtt_publisher.c`**: Task stack 4096 → **6144** (mais espaço para `simcom_driver_mqtt_publish()` com `char cmd[512]`)

### Tabela de Prioridades Final

| Task | Prio | Core | Prioridade |
|------|:----:|:----:|------------|
| `dispatcher_task` | **6** | 0 | 🔝 RFID |
| `stm32_uart_rx_task` | **6** | 0 | 🔝 RX STM32 |
| `simcom_rx` | **6** | 1 | 🔝 RX Modem |
| `system_orchestrator` | **5** | 0 | Orquestração (agora non-blocking) |
| `mqtt_pub_task` (6144 stk) | **4** | 1 | Publicação MQTT (única que toca o driver) |
| `simcom_wd` | **3** | 1 | Watchdog modem |
| `ota_task` | **5**→**4** | 1 | OTA (evento raro) |
| `cache_sync_task` | **3**→**2** | 1 | Cache offline (só enfileira) |
| `log_processor` | **3**→**2** | 1 | Logs |
| `telnet_logger` | **3**→**2** | 1 | Debug |

**Resultado:** Apenas 1 task (`mqtt_pub_task`) e o orchestrator (non-blocking) competem pelo `simcom_mutex`. RFID no topo absoluto. GPS e time sync não travam mais.

---

## Sessão 35 — Documentação Completa de OTA
**Data:** 2026-06-25
**Objetivo:** Mapear e documentar a capacidade OTA atual do projeto, limitações e próximos passos.

### Entregas

- **PROJETO_BASTAO.md**: Nova seção "Atualização OTA" com arquitetura de partições A/B, fluxo de disparo MQTT, tabela de limitações conhecidas (Wi-Fi apenas, sem K10, sem FOTA, sem CI/CD), geração de binário e teste.
- **Manual/funcionalidades.md**: Seção 2.6 corrigida — removida menção incorreta a "4G", adicionado escopo real (ESP32 apenas), esclarecido funcionamento do rollback.
- **Manual/arquitetura.md**: Task OTA Manager atualizada com notas sobre partições duais e rollback via `esp_ota_mark_app_valid_cancel_rollback()`.
- **ROADMAP.md**: Adicionadas 3 fases planejadas: Fase 44 (OTA via 4G), Fase 45 (OTA na K10), Fase 46 (CI/CD + report de progresso).
- **COMPILATION_GUIDE.md**: Nova seção 5 com instruções de geração de binário OTA, hospedagem HTTPS e disparo.
- **README.md**: Indicadores de limitação OTA adicionados à feature list.
- **progress.md**: Phase 47 registrada.
- **AGENTS.md**: Esta sessão.

### Diagnóstico da Capacidade OTA

| Dispositivo | OTA Funcional? | Como? | Limitação |
|-------------|:--------------:|-------|-----------|
| ESP32 Coordenador | ✅ | HTTPS (Wi-Fi) | ❌ Não funciona via 4G |
| Tela K10 | ❌ | — | Partição única `factory` |
| Modem SIMCom | ❌ | — | AT+CFOTA não implementado |

---

## Sessão 36 — Correção: CMQTTREL com retry (PUBACK do QoS 1 ainda pendente)
**Data:** 2026-06-25
**Objetivo:** Corrigir `AT+CMQTTREL=0` que falhava com erro 14 ("client is busy") após `AT+CMQTTPUB` com QoS 1.

### Problema
- `AT+CMQTTPUB=0,1,60` retorna OK imediatamente (modem aceitou o publish), mas o message ID 0 fica **ocupado** até o PUBACK do broker chegar.
- `AT+CMQTTREL=0` enviado logo após o OK do CMQTTPUB sempre falha com `+CMQTTREL: 0,14` (erro 14 = busy).
- O CMQTTREL no início do próximo publish (Sessão 33) tentava uma única vez com 1s de timeout — se o PUBACK demorasse >1s, falhava também.
- Resultado: publish seguinte tentava `CMQTTTOPIC=0,34` com message ID 0 ocupado → `+CMQTTTOPIC: 0,14` ERROR → ciclo de falhas.

### Modificações
- `simcom_driver_mqtt_publish()` em `simcom_driver.c`:
  - **Removido** `CMQTTREL` depois do `CMQTTPUB` (era inútil/muito cedo).
  - **Adicionado** retry loop no início da função: 10 tentativas a cada 300ms (total ~3s) de `CMQTTREL=0` até sucesso.
  - O PUBACK típico chega em 200-800ms, então o retry loop cobre o caso com folga.

## Sessão 37 — Correção: Payload MQTT `nivel_bateria` enviava tensão em vez de percentual

---

## Sessão 38 — Ativação do RFID Dedup (janela de 1 minuto)
**Data:** 2026-06-25
**Objetivo:** Não enviar a mesma tag RFID repetidamente dentro do intervalo de 1 minuto.

### Problema
- `rfid_dedup_is_duplicate()` era inicializada mas **NUNCA chamada** no `dispatcher_task`. Toda tag — mesmo idêntica lida 10x em 1s — passava pelo pipeline completo: DB lookup, BLE Mesh, criptografia AES, MQTT publish.
- Janela de dedup era 10s, mas o requisito é 1 minuto.

### Modificações
- `rfid_dedup.h`: `RFID_DEDUP_WINDOW_MS` 10000 → **60000** (1 minuto)
- `main.c` dispatcher_task: Adicionado `rfid_dedup_is_duplicate()` logo após o check de tipo RFID. Se duplicata, `continue` — pula totalmente o processamento (Mesh + MQTT).

---

## Sessão 37 — Correção: Payload MQTT `nivel_bateria` enviava tensão em vez de percentual
**Data:** 2026-06-25
**Objetivo:** Corrigir `nivel_bateria` no payload MQTT que enviava tensão bruta (ex: `8.50`) em vez do percentual 0-100.

### Problema
- O payload `nivel_bateria` em `main.c:122` usava `bastao_current_status.battery_voltage` (tensão bruta) diretamente formatado como `%.2f`.
- O cálculo de percentual (`pct`) existia (linhas 132-135: 7.50V=0%, 8.80V=100%) mas só era usado no payload da BLE Mesh, nunca no MQTT.
- A tela K10 mostrava o percentual correto porque recebia `{"type":"batt","volt":x,"pct":y}` via Mesh, mas a nuvem recebia `nivel_bateria`: 8.50.

### Modificações
- `esp32_firmware/main/main.c`: Linhas 119-122 — `batt` (float, tensão) substituído por `batt_pct` (int, percentual). Mapeamento: 7.50V→0%, 8.80V→100%.

---

## Sessão 39 — Otimização de Throughput RFID + Telnet Debug
**Data:** 2026-06-25
**Objetivo:** Eliminar bottlenecks que causavam lentidão na leitura das tags RFID e habilitar debug via Telnet para monitoramento remoto.

### Problemas Resolvidos

**1. AES-256 encryption bloqueava dispatcher_task (Core 0, prio 6):**
- `secure_payload_encrypt()` era chamada **inline** no `dispatcher_task` para cada tag RFID lida. AES-256-CBC leva 5-50ms por tag, bloqueando o processamento de novas tags durante esse período.
- Se 2+ tags chegassem em rápida sucessão, a fila `stm32_data_queue` (depth 50) enchia e tags eram perdidas.

**2. Telnet Logger não capturava logs do ESP-IDF:**
- `esp_logger_write()` criava uma fila de logs própria (`log_queue`), mas **ninguém chamava** essa função. Todo o sistema usa `ESP_LOGI()`, `ESP_LOGE()` etc., que iam direto para UART via `vprintf()` padrão do ESP-IDF.
- Conectar via Telnet não mostrava absolutamente nada.

**3. stm32_uart_rx_task e dispatcher_task no mesmo Core (0) com mesma prioridade (6):**
- Ambas as tasks rodavam no Core 0 com prioridade 6, causando contenção de CPU.
- `uart_read_bytes()` com timeout de 20ms no RX task bloqueava o dispatcher indiretamente.

**4. cJSON pesado no RX task de alta prioridade:**
- O parsing de cada JSON com `cJSON_Parse()` (malloc/free) era executado na task de RX (prio 6), ocupando CPU preciosa.

### Modificações

**1. AES Encryption movido para mqtt_publish_task (Core 1, prio 4):**
- `esp32_firmware/main/mqtt_publisher.h`: Adicionado campo `needs_encrypt` em `mqtt_publish_msg_t`
- `esp32_firmware/main/mqtt_publisher.h`: Adicionado protótipo `mqtt_publisher_enqueue_raw()`
- `esp32_firmware/main/mqtt_publisher.c`: Adicionado `#include "secure_payload.h"`
- `esp32_firmware/main/mqtt_publisher.c`: `mqtt_publish_task()` agora chama `secure_payload_encrypt()` antes de publicar/salvar no cache
- `esp32_firmware/main/mqtt_publisher.c`: `mqtt_publisher_enqueue_raw()` — raw JSON com `needs_encrypt=true`
- `esp32_firmware/main/mqtt_publisher.c`: `mqtt_publisher_enqueue()` — pre-encrypted com `needs_encrypt=false` (backward compat)
- `esp32_firmware/main/main.c`: `dispatcher_task` — removido `encrypted_hex[768]` da stack, removido `secure_payload_encrypt()`, agora chama `mqtt_publisher_enqueue_raw()` com JSON cru.
- **Resultado:** dispatcher task leva apenas ~1ms para enfileirar, libera CPU imediatamente para próxima tag.

**2. Telnet Logger com vprintf hook:**
- `esp32_firmware/main/esp32_logger.c`: Adicionada função `telnet_vprintf()` que captura TODAS as saídas formatadas do ESP-IDF (ESP_LOGI/LOGE/etc)
- `esp32_firmware/main/esp32_logger.c`: Hook registrado com `esp_log_set_vprintf(telnet_vprintf)` em `esp_logger_init()`
- `esp32_firmware/main/esp32_logger.c`: O hook mantém saída UART original + broadcast para clientes Telnet
- `esp32_firmware/main/esp32_logger.c`: Stack da task telnet aumentada 4096→6144
- **Resultado:** `telnet <esp32_ip>` mostra todos os logs em tempo real.

**3. stm32_uart_rx_task movido para Core 1:**
- `esp32_firmware/main/stm32_uart.c`: `xTaskCreatePinnedToCore(..., 0)` → `(..., 1)`
- **Resultado:** RX UART no Core 1, dispatcher_task no Core 0 — zero contenção.

### Fluxo Novo de Leitura de Tag (sem bloqueio AES no dispatcher)
```
STM32 → UART → stm32_uart_rx_task (Core 1, prio 6)
  → Queue → dispatcher_task (Core 0, prio 6): 
    → dedup check (~1μs)
    → animal_db_lookup (~1ms)
    → mesh_coordinator_send_data (non-blocking)
    → mqtt_publisher_enqueue_raw (QUEUE, ~1μs) ← SEM AES AQUI
      → Queue → mqtt_pub_task (Core 1, prio 4):
        → secure_payload_encrypt (5-50ms) ← AES AQUI
        → publish/cache
```

### Conectando via Telnet
```bash
telnet <ip_do_esp32>   # porta 23
# Todos os logs ESP_LOGI/LOGE aparecem em tempo real
```

---

## Sessão 40 — Correção YRM100: Configuração de Região + Potência TX
**Data:** 2026-06-26
**Objetivo:** Resolver leitura do YRM100 que não lia tags por falta de configuração de região e potência RF.

### Problemas Resolvidos
- **Região de frequência nunca configurada (0x07):** O YRM100 saía de fábrica configurado para China (920-925MHz), mas o Brasil usa faixa FCC 902-928MHz (US/América, parâmetro 0x02). Sem essa configuração, o módulo transmitia em frequências erradas e não energizava as tags.
- **Potência TX removida (Sessão 38):** `YRM100_SetTXPower(26)` foi removido — módulo operava em potência padrão (possivelmente 0dBm), insuficiente para leitura.
- **Sem handler dinâmico:** ESP32 não tinha como ajustar potência do YRM100 em tempo real.

### Análise Comparativa
| Característica | WL-134 (LF) ✅ | YRM100 (UHF) ❌ Antes |
|---|---|---|
| Protocolo | ASCII 30 bytes fixos | Binário variável |
| Polling | Não (automático) | Sim (comando 0x22) |
| Configuração inicial | Nenhuma | Região (0x07) + Potência (0xB6) |
| Alimentação | Só ligar | Precisa estabilização RF + região correta |

### Modificações

**STM32 (`rfid_parser.h`):**
- Adicionado `YRM100_Init(void)` — sequência de inicialização
- Adicionado `YRM100_SetRegion(uint8_t region)` — comando 0x07
- Adicionado `YRM100_SetTXPower(uint8_t dbm)` — comando 0xB6 (dbm → centésimos ×100)
- Adicionado `YRM100_StartContinuousRead(void)` — comando 0x27 (Multiple Inventory)
- Adicionado `YRM100_StopContinuousRead(void)` — comando 0x28

**STM32 (`rfid_parser.c`):**
- `YRM100_SetRegion()`: Monta frame `BB 00 07 00 01 <region> <CS> 7E`, aguarda resposta
- `YRM100_SetTXPower()`: Monta frame `BB 00 B6 00 02 <power_h> <power_l> <CS> 7E`, aguarda resposta
- `YRM100_StartContinuousRead()`: Monta frame `BB 00 27 00 03 22 FF FF <CS> 7E` — 65535 ciclos
- `YRM100_StopContinuousRead()`: Monta frame `BB 00 28 00 00 28 7E`
- `YRM100_Init()`: Chama SetRegion(0x02) → delay → SetTXPower(20dBm) → **StartContinuousRead()**

**STM32 (`main.c`):**
- Chamada `YRM100_Init()` após `HAL_Delay(100)` no boot
- **Removido** polling periódico de 0x22 (single inventory) a cada 100ms
- **Removida** variável `last_yrm100_poll`

**STM32 (`alerts.c`):**
- Handler `{"cmd":"yrm_tx_power","value":20}` — para, ajusta potência, reinicia inventário
- Handler `{"cmd":"yrm_restart"}` — reinicia o módulo completo

### Mudança de Arquitetura: Single Poll → Continuous Read

| Antes (0x22 poll) | Depois (0x27 contínuo) |
|---|---|
| Envia comando a cada 100ms | PA fica ligado continuamente |
| 10 ciclos/s máx | Até 30-50 ciclos/s |
| PA liga/desliga a cada ciclo | PA sempre ligado (mais rápido) |
| Perde tags entre polls | Detecta tags em tempo real |
| `HAL_UART_Transmit` bloqueante a cada 100ms | Sem TX periódico (só RX) |

### Sequência Correta de Inicialização
1. **Set Mode** → `BB 00 F5 00 01 01 F7 7E` (modo de operação)
2. **Set Region** → `BB 00 07 00 01 02 0A 7E` (US/América 902-928 MHz)
3. **Set TX Power** → `BB 00 B6 00 02 07 D0 8F 7E` (20 dBm)
4. **Save Config** → `BB 00 09 00 01 01 0B 7E` (persiste na flash do módulo)
5. **Multiple Inventory** → `BB 00 27 00 03 22 FF FF 4A 7E` (65535 ciclos)

### Comandos Configurados
- **Região:** `0x02` (US/América, 902-928 MHz) — compatível com ANATEL Brasil
- **Potência:** 20 dBm (2000 centésimos = `0x07D0`)
- **Frame Set Mode:** `BB 00 F5 00 01 01 F7 7E`
- **Frame Set Region:** `BB 00 07 00 01 02 0A 7E`
- **Frame Set Power:** `BB 00 B6 00 02 07 D0 8F 7E`
- **Frame Save Config:** `BB 00 09 00 01 01 0B 7E`
- **Frame Multiple Inventory:** `BB 00 27 00 03 22 FF FF 4A 7E`

---

## Sessão 41 — GPS Adaptativo: Polling 2s (busca) / 30s (fix)
**Data:** 2026-06-26
**Objetivo:** Acelerar aquisição do sinal GPS com polling a cada 2s até obter fix, depois reduzir para 30s.

### Problema
- GPS era lido sempre a cada 30s, independente de ter fix ou não
- Se o GPS perdia sinal (ou no boot), demorava até 30s para a primeira tentativa de aquisição

### Solução
- **Sem fix:** `system_orchestrator_task` lê GPS a cada **2 segundos** (busca rápida)
- **Com fix:** após `gps_fix = true`, intervalo muda automaticamente para **30 segundos**
- A transição é automática e logada: `"GPS FIX OBTIDO! Mudando para polling de 30s."`
- O intervalo é calculado a cada ciclo: `uint32_t gps_interval = bastao_current_status.gps_fix ? 30 : 2;`
- Mantido o guard `!simcom_driver_is_busy()` para não travar o modem durante MQTT

### Modificações
**`esp32_firmware/main/main.c`** (`system_orchestrator_task`):
- Substituído `gps_tick >= 30` fixo por `gps_tick >= gps_interval` adaptativo
- Adicionada variável local `simcom_gps_data_t gps_new` para leitura atômica
- Log de transição quando fix é obtido
- Estrutura do bloco GPS mantida (Mesh update + gateway status continuam atrelados ao ciclo de leitura)

### Fluxo
```
Boot → gps_fix = false → poll de 2 em 2s
  → simcom_driver_get_gps() → ESP_OK && valid
  → gps_fix = true → "GPS FIX OBTIDO!"
  → poll passa automaticamente para 30s
```

---

## Sessão 42 — Otimização FreeRTOS: Prioridades, Stacks e Monitoramento
**Data:** 2026-06-26
**Objetivo:** Eliminar contenção entre SIMCom/RFID e acelerar atualização da tela K10.

### Problemas Identificados

**Contenção SIMCom vs RFID (Coordinator):**
- `cache_sync_task` (prio 2) drenava cache muito lentamente quando MQTT voltava
- `ota_task` (prio 5) tinha prioridade excessiva para evento raro
- `stm32_uart_rx_task` (stack 4096) e `mqtt_pub_task` (stack 6144) podiam estar no limite com buffers JSON/AES
- Nenhum monitoramento de stack — estouro passava despercebido

**Lentidão da tela (K10):**
- Poll de dados Mesh a cada **200ms** — tag demorava até 200ms para aparecer
- `network_task` a **prio 5** para task ociosa (só delay de 1s)

### Modificações

**ESP32 Coordinator (`main.c`):**
- `offline_cache_sync_task_start(2)` → `(3)` — drain mais rápido do cache
- Adicionado `uxTaskGetStackHighWaterMark` para 9 tasks no boot

**ESP32 Coordinator (`stm32_uart.c`):**
- Stack da `stm32_uart_rx_task`: 4096 → **5120** bytes

**ESP32 Coordinator (`mqtt_publisher.c`):**
- Stack da `mqtt_pub_task`: 6144 → **8192** bytes (AES + JSON)

**ESP32 Coordinator (`ota_manager.c`):**
- Prioridade da `ota_task`: 5 → **4** (evento raro)

**K10 (`main.c`):**
- Poll de dados Mesh: `sensor_timer >= 20` (200ms) → `>= 5` (**50ms**)
- Prioridade da `network_task`: 5 → **1** (task ociosa)
- Adicionado `uxTaskGetStackHighWaterMark` no boot

### Tabela de Prioridades Final

**Coordinator:**
| Prio | Core 0 | Core 1 |
|:----:|:-------|:-------|
| **6** | `dispatcher_task` (RFID) | `stm32_uart_rx_task`, `simcom_rx` |
| **5** | `system_orchestrator` | — |
| **4** | — | `mqtt_pub_task`, `ota_task` (eventual) |
| **3** | — | `simcom_wd`, `log_processor`, `telnet_logger`, `cache_sync_task`, **`gps_reader`** |
| **2** | — | — |

**K10:**
| Prio | Core 0 | Core 1 |
|:----:|:-------|:-------|
| **5** | — | `gui_task` (LVGL + sensores) |
| **1** | `network_task` (ociosa) | — |

---

## Sessão 43 — GPS em Task Separada + Correção de Polling
**Data:** 2026-06-26
**Objetivo:** Corrigir GPS que não lia a cada 2s e parar de travar o loop do orchestrator.

### Problema Raiz
O `system_orchestrator_task` rodava a cada 1s (`vTaskDelay(1000)`), mas `simcom_driver_get_gps()` bloqueava por **~1-2s** (AT+CGPSINFO + AT+CGNSSINFO sem fix). Resultado: o loop inteiro levava ~2-3s, e com `gps_interval=2` o GPS era lido a cada **~4-6s** em vez de 2s.

### Solução
Criada `gps_reader_task` (prio 3, Core 1, stack 4096) separada que:
1. Aguarda notificação do orchestrator via `xTaskNotify`
2. Chama `simcom_driver_get_gps()` em background (pode bloquear à vontade)
3. Atualiza `gps_data` e `bastao_current_status`
4. Sinaliza `gps_read_done` quando termina

O orchestrator agora:
1. Incrementa `gps_tick` a cada 1s (não bloqueia mais)
2. Quando `gps_tick >= gps_interval`, envia notificação para a GPS task
3. No próximo ciclo, se `gps_read_done`, envia dados para K10 via Mesh
4. Loop roda sempre a **~1s** independente do GPS

### Fluxo Novo
```
Orchestrator (1s loop):            GPS Reader Task (background):
  gps_tick++                         xTaskNotifyWait(5000ms)
  if (tick >= intervalo) {           if notified:
    xTaskNotify(gps_reader)            simcom_driver_get_gps()
    gps_read_pending = true              → bloqueia 1-3s (OK aqui)
  }                                     atualiza gps_data
  if (gps_read_done):                   gps_read_done = true
    envia Mesh }                      }
```

### Arquivos Modificados
- `main.c`: Adicionada `gps_reader_task`, globais de sincronização, criação da task em `app_main`
- `AGENTS.md`: Esta sessão

---

## Sessão 44 — Correções Finais: Nomes de Task, CMQTTREL, WiFi AP, BLE Mesh Segments + Documentação
**Data:** 2026-06-26
**Objetivo:** Corrigir travamento do ESP32 (assert xTaskGetHandle), retry loop MQTT, ordem de init do WiFi AP, BLE Mesh segment exhaustion, e gerar documentação completa do projeto.

### Problemas Resolvidos

**Bug 1 — Assert xTaskGetHandle (nome de task > 15 chars):**
- `xTaskGetHandle()` no FreeRTOS requer nomes com strlen < 16. 
- `"stm32_uart_rx_task"` (18) e `"system_orchestrator"` (19) estouravam o limite.
- Corrigido: `stm32_uart_rx_task` → `stm32_uart_rx`, `system_orchestrator` → `sys_orchestr`

**Bug 2 — CMQTTREL sem retry (MQTT falhando):**
- `AT+CMQTTREL=0` era tentado uma única vez, mas com QoS 1 o message ID 0 fica ocupado até PUBACK.
- Adicionado retry loop: 10 tentativas a cada 300ms (3s totais).

**Bug 3 — WiFi AP nao aparecia:**
- `start_softap_after_init()` chamava `esp_wifi_set_config(AP)` **depois** de `esp_wifi_start()`.
- ESP-IDF exige config antes de start. Movido `start_softap_after_init()` antes do `esp_wifi_start()`.

**Bug 4 — BLE Mesh segment exhaustion:**
- `CONFIG_BLE_MESH_TX_SEG_MSG_COUNT` e `RX_SEG_MSG_COUNT` estavam em 4, insuficientes para envio simultâneo de GPS + cell + status.
- Aumentado para 12 em ambos (ESP32 e K10).

**Bug 5 — YRM100 nao lia tags (STM32):**
- RX do USART4 era ativado DEPOIS do `YRM100_Init()`, perdendo respostas dos comandos de configuração.
- Movido `HAL_UART_Receive_IT(&huart4)` para ANTES do init.
- Trocado continuous read (0x27) para single poll (0x22) a cada 100ms (menos sobrecarga no buffer).

**Bug 6 — STM32006 travando no boot (YRM100_ReportConfig):**
- `YRM100_ReportConfig()` era chamado após `YRM100_StartContinuousRead()`, mas dados de tag contínuos lotavam o buffer e impediam leitura das respostas dos comandos de configuração.
- Movido `YRM100_ReportConfig()` para DENTRO do `YRM100_Init()`, ANTES do `StartContinuousRead()`.

### Funcionalidades Adicionadas
- **Modo de teste YRM100** (`#define YRM100_TEST_MODE`): loop de poll a cada 500ms com dump hex para ESP32.
- **Report de config YRM100**: STM32 le firmware, região e potência e envia JSON para ESP32 no boot.
- **Wake da K10 por tag**: `hal_display_reset_inactivity()` chamado no recebimento de RFID via Mesh.
- **Timeout display K10**: 120s (2 minutos), configurado em `hal_display.c`.
- **Documentação completa**: `documentacao/` com 6 arquivos (plano, arquitetura, infra, segurança, agentes, roadmap).

### Modificações

**ESP32 (`esp32_firmware/main/main.c`):**
- Nomes das tasks no array `task_names[]` corrigidos para ≤ 15 chars
- `xTaskCreatePinnedToCore("system_orchestrator"` → `"sys_orchestr"`

**ESP32 (`esp32_firmware/main/stm32_uart.c`):**
- `xTaskCreatePinnedToCore("stm32_uart_rx_task"` → `"stm32_uart_rx"`
- Adicionado handler para `"yrm100_cfg"` e `"yrm100_test"`

**ESP32 (`esp32_firmware/main/simcom_driver.c`):**
- `simcom_driver_mqtt_publish()`: Adicionado retry loop de 10×300ms para `AT+CMQTTREL=0`

**ESP32 (`esp32_firmware/main/wifi_driver.c`):**
- `start_softap_after_init()` movido para **antes** de `esp_wifi_start()`

**ESP32 (`esp32_firmware/sdkconfig.defaults` + `sdkconfig`):**
- `CONFIG_BLE_MESH_TX_SEG_MSG_COUNT=4` → `12`
- `CONFIG_BLE_MESH_RX_SEG_MSG_COUNT=4` → `12`

**K10 (`k10_firmware/sdkconfig.defaults` + `sdkconfig`):**
- `CONFIG_BLE_MESH_TX_SEG_MSG_COUNT=4` → `12`
- `CONFIG_BLE_MESH_RX_SEG_MSG_COUNT=4` → `12`

**STM32 (`stm32_firmware/Core/Src/main.c`):**
- `HAL_UART_Receive_IT(&huart4)` movido para ANTES de `YRM100_Init()`
- `YRM100_ReportConfig()` removido (agora dentro de `YRM100_Init()`)
- Adicionado `YRM100_SinglePoll()` a cada 100ms no loop principal
- Adicionado `#define YRM100_TEST_MODE` (comentado) para testes
- Adicionada variável `last_yrm100_poll` e `YRM100_POLL_MS`

**STM32 (`stm32_firmware/Core/Src/rfid_parser.c` + `.h`):**
- Adicionado `YRM100_ReportConfig()` — lê firmware, região, potência e envia JSON
- Adicionado `YRM100_ReadFirmwareVersion()`, `YRM100_ReadRegion()`, `YRM100_ReadTXPower()`
- Adicionado `YRM100_SinglePoll()` — comando 0x22 single inventory
- Adicionado `YRM100_FlushBuffer()` e `YRM100_ReadRawBuffer()` para diagnóstico
- Removido `StartContinuousRead()` do init (substituído por single poll)

### Documentação Criada
- `documentacao/00_Plano_de_Implementacao.md` — Visão geral, stack, fases, pinagem
- `documentacao/01_Arquitetura_do_Sistema.md` — Fluxo de dados, tasks, protocolos BLE Mesh
- `documentacao/02_Infraestrutura_e_Docker.md` — Build, deploy, Docker, ambiente
- `documentacao/03_Multi_Tenancy_e_Seguranca.md` — Criptografia, BLE, MQTT, multi-tenancy
- `documentacao/04_Agentes_e_Desenvolvimento.md` — Agents, skills, padrões, git
- `documentacao/08_Roadmap.md` — Cronograma, fases, próximos passos
- `README.md` atualizado para apontar para `documentacao/`

---

---

## Sessão 45 — GPS: Cold Start (Hot Start nao funcionava)
**Data:** 2026-06-26
**Objetivo:** Reverter GPS de Hot Start (AP_Flash) para Cold Start por incompatibilidade com o modem A7670C/A7663E.

### Problema
- `AT+CGNSSPWR=1,1` (Hot Start com AP_Flash) nao funcionava consistentemente no modem
- GPS nunca obtinha fix porque as efemerides nao eram salvas/carregadas corretamente
- Modo Hot Start exige chip GPS ASR1601 com suporte a AP_Flash, mas o comportamento varia entre firmware

### Solucao
- `simcom_driver_gps_power_on()`: `AT+CGNSSPWR=1,1` → `AT+CGNSSPWR=1` (Cold Start)
- `simcom_driver_gps_power_off()`: `AT+CGNSSPWR=0,1` → `AT+CGNSSPWR=0`
- Removido fallback triplo: agora tenta CGNSSPWR=1, se falhar → CGPS=1

### Arquivos Modificados
- `esp32_firmware/main/simcom_driver.c`: `simcom_driver_gps_power_on()` e `simcom_driver_gps_power_off()`
- `documentacao/01_Arquitetura_do_Sistema.md`: Adicionada secao GPS com detalhes de cold start e polling

---

## Referência Rápida de Comandos

```powershell
# Build ESP32
idf.py -C D:\git\Bastao\Bastao-ESP\esp32_firmware build

# Build K10
idf.py -C D:\git\Bastao\Bastao-ESP\k10_firmware build

# Build STM32 (Release)
$env:Path = "C:\ST\STM32CubeCLT_1.16.0\GNU-tools-for-STM32\bin;$env:Path"
cd D:\git\Bastao\Bastao-ESP\stm32_firmware\Release
& "C:\ST\STM32CubeIDE_1.16.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.1.300.202402091052\tools\bin\make.exe" main-build

# Flash + Monitor (ESP32)
idf.py -p COM7 flash monitor

# Limpar build
Remove-Item -Recurse -Force build
```
