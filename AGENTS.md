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
