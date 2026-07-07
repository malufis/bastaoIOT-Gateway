# Registro de Progresso: Bastão-ESP

## Session: 2026-05-19

### Phase 1: Estruturação Física e Diretórios
- **Status:** complete
- **Started:** 2026-05-19T11:07:00
- Actions taken:
  - Inicializados os arquivos de controle de plano de tarefas (`task_plan.md`), descobertas (`findings.md`) e progresso (`progress.md`).
  - Criadas as pastas `teste_automatizado/` e `debug/` com seus respectivos arquivos `.gitkeep`.
- Files created/modified:
  - `task_plan.md` (criado)
  - `findings.md` (criado)
  - `progress.md` (criado)
  - `teste_automatizado/.gitkeep` (criado)
  - `debug/.gitkeep` (criado)

### Phase 2: Documentação de Manuais Humanos
- **Status:** complete
- **Started:** 2026-05-19T11:07:30
- Actions taken:
  - Criados os manuais técnicos humanos `Manual/git_guidelines.md`, `Manual/arquitetura.md` e `Manual/funcionalidades.md`.
- Files created/modified:
  - `Manual/git_guidelines.md` (criado)
  - `Manual/arquitetura.md` (criado)
  - `Manual/funcionalidades.md` (criado)
  - `progress.md` (modificado)

### Phase 3: Base de Conhecimento de Protocolos (aprendizado)
- **Status:** complete
- **Started:** 2026-05-19T11:08:10
- Actions taken:
  - Criado o arquivo `aprendizado/rfid_protocols.md` compilando a especificação detalhada de decodificação do YRM100 e WL-134 para IAs.
- Files created/modified:
  - `aprendizado/rfid_protocols.md` (criado)
  - `progress.md` (modificado)

### Phase 4: Atualização e Criação dos Perfis de Agente
- **Status:** complete
- **Started:** 2026-05-19T11:08:30
- Actions taken:
  - Atualizado o perfil do `.agents/rfid_agent.md` com referências aos protocolos de baixo nível.
  - Criado o novo perfil de agente `.agents/stm32_agent.md` para suportar o desenvolvimento da MCU STM32G070CBTx.
- Files created/modified:
  - `.agents/rfid_agent.md` (modificado)
  - `.agents/stm32_agent.md` (criado)
  - `progress.md` (modificado)

### Phase 5: Atualização do README Geral
- **Status:** complete
- **Started:** 2026-05-19T11:08:50
- Actions taken:
  - Criado o `README.md` raiz com o escopo resumido, equipamentos, placas e frameworks utilizados no projeto.
- Files created/modified:
  - `README.md` (criado)
  - `progress.md` (modificado)

### Phase 6: Comunicação Bluetooth no ESP32
- **Status:** complete
- **Started:** 2026-05-19T11:25:30
- Actions taken:
  - Adicionado suporte a documentação de Bluetooth com celulares nos manuais `Manual/arquitetura.md` e `Manual/funcionalidades.md`.
  - Atualizado o perfil do agente de conectividade `.agents/esp32_connectivity_agent.md` com as diretrizes e responsabilidades do Bluetooth Mobile.
- Files created/modified:
  - `Manual/arquitetura.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `.agents/esp32_connectivity_agent.md` (modificado)
  - `progress.md` (modificado)

### Phase 7: Atualização Remota (OTA) no ESP32
- **Status:** complete
- **Started:** 2026-05-19T11:31:40
- Actions taken:
  - Adicionado suporte a documentação de atualização remota (OTA) via Wi-Fi e 4G nos manuais `Manual/arquitetura.md` e `Manual/funcionalidades.md`.
  - Atualizado o perfil do agente do ESP32 `.agents/esp32_connectivity_agent.md` com a responsabilidade de gerenciar as partições e rollback do OTA.
- Files created/modified:
  - `Manual/arquitetura.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `.agents/esp32_connectivity_agent.md` (modificado)
  - `progress.md` (modificado)

### Phase 8: Sistema de Cache de Dados Offline
- **Status:** complete
- **Started:** 2026-05-19T11:33:15
- Actions taken:
  - Adicionado suporte a documentação de buffer local e persistência em flash (LittleFS/SPIFFS) nos manuais `Manual/arquitetura.md` e `Manual/funcionalidades.md`.
  - Atualizado o perfil do agente do ESP32 `.agents/esp32_connectivity_agent.md` com a responsabilidade do spooler offline FIFO.
- Files created/modified:
  - `Manual/arquitetura.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `.agents/esp32_connectivity_agent.md` (modificado)
  - `progress.md` (modificado)

## Test Results
| Test | Input | Expected | Actual | Status |
|------|-------|----------|--------|--------|
| Inicialização de arquivos | Criação de task_plan.md, findings.md e progress.md | Arquivos existem e contêm templates estruturados | Criados e populados com sucesso | ✓ |
| Criação de diretórios | Criação física de teste_automatizado e debug | Pastas existem no sistema com arquivos .gitkeep | Criadas com sucesso no workspace | ✓ |
| Criação de Manuais | Escrita de git_guidelines.md, arquitetura.md e funcionalidades.md | Manuais criados contendo a documentação humana correspondente | Criados com sucesso na pasta Manual/ | ✓ |
| Base de Conhecimento RFID | Escrita de rfid_protocols.md | Base contendo especificações exatas de decodificação LSB e checksum | Criado com sucesso na pasta aprendizado/ | ✓ |
| Criação de Agentes | Escrita de rfid_agent.md e stm32_agent.md | Perfis atualizados com pinagem, protocolos e boas práticas do STM32G070 | Criados com sucesso na pasta .agents/ | ✓ |
| Escrita do README | Escrita do README.md raiz | Arquivo criado apresentando o projeto e a estrutura do repositório | Criado com sucesso no workspace | ✓ |
| Ajuste de Bluetooth | Adição de dados Bluetooth nos manuais e agente do ESP32 | Manuais e perfis atualizados com o GATT Server e sincronização de cadastro de fazenda, lote e animais | Atualizado com sucesso | ✓ |
| Ajuste de OTA | Adição de dados OTA nos manuais e agente do ESP32 | Manuais e perfis updated com download seguro HTTPS, dupla partição e seleção Wi-Fi/4G | Atualizado com sucesso | ✓ |
| Ajuste de Cache Offline | Adição de cache local na flash nos manuais e agente do ESP32 | Manuais e perfis atualizados com spooler FIFO, monitoramento de conexões e sincronização posterior | Atualizado com sucesso | ✓ |

## Error Log
| Timestamp | Error | Attempt | Resolution |
|-----------|-------|---------|------------|
| 2026-05-19T10:45 | Error invalid tool call (IsArtifact fora do cérebro) | 1 | Substituição por IsArtifact = false e escrita normal de arquivo |

## Session: 2026-05-20

### Phase 9: Criptografia de Payload e Despachante ESP32
- **Status:** complete
- **Started:** 2026-05-20T12:53:00
- Actions taken:
  - Criado o módulo `secure_payload.c/.h` implementando criptografia simétrica AES-256-CBC com padding PKCS#7 via `mbedtls` no ESP32.
  - Criado o esqueleto do módulo `mesh_coordinator.c/.h` para a inicialização e envio de dados criptografados pela rede BLE Mesh do ESP32.
  - Atualizado o orquestrador `main.c` com a inicialização do NVS Flash, módulo de segurança, módulo Mesh, e com a tarefa despachante (`dispatcher_task`) que consome a fila UART do STM32, formata os JSONs, realiza a criptografia e envia pela rede.
  - Atualizado o arquivo `CMakeLists.txt` do ESP32 para incluir os novos arquivos de código-fonte.
  - Adicionado o script de teste criptográfico `teste_automatizado/verify_encryption.py` utilizando `pycryptodomex` para certificar a compatibilidade dos payloads criptografados.
- Files created/modified:
  - `esp32_firmware/main/secure_payload.h` (criado)
  - `esp32_firmware/main/secure_payload.c` (criado)
  - `esp32_firmware/main/mesh_coordinator.h` (criado)
  - `esp32_firmware/main/mesh_coordinator.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_encryption.py` (criado)
  - `findings.md` (modificado)

## Test Results
| Test | Input | Expected | Actual | Status |
|------|-------|----------|--------|--------|
| Teste de Decodificação RFID | Frames brutos de YRM100 e WL-134 | EPCs decodificados e convertidos corretamente para decimais | Todos decodificados conforme especificação | ✓ |
| Teste de Criptografia AES | Strings JSON estruturadas | Payload criptografado hex compatível com padrão decifrável no receptor | Sucesso na cifragem e decifração de teste | ✓ |

## 5-Question Reboot Check
| Question | Answer |
|----------|--------|
| Where am I? | Finalizado modulo BLE Mobile GATT Server no ESP32 |
| Where am I going? | Implementar Task OTA Manager (atualizacao remota de firmware) |
| What's the goal? | Garantir a transmissao fim a fim (local via Mesh e remota via Celular) criptografada, com sincronizacao bidirecional via app |
| What have I learned? | Ver detalhes em findings.md |
| What have I done? | Concluí o modulo ble_mobile com GATT Server seguro, config/business write, tag notify, status read e NVS. |

### Phase 10: SIMCom 7663E -- Driver Celular PPP + MQTT
- **Status:** complete
- **Started:** 2026-05-20T13:08:00
- Actions taken:
  - Criado o modulo `simcom_ppp.c/.h` com engine de comandos AT, ativacao PPP via ESP-NETIF, leitura GPS/GLONASS e watchdog de reconexao.
  - Criado o modulo `mqtt_publisher.c/.h` com cliente MQTT nativo do ESP-IDF, fila dedicada de publicacao e task consumidora assincrona.
  - Atualizado o orquestrador `main.c` com a inicializacao do modem SIMCom, PPP, cliente MQTT e roteamento paralelo de dados (BLE Mesh + MQTT).
  - Adicionadas configuracoes padrao de APN (Vivo) e broker MQTT como constantes de compilacao.
  - Criado o teste `verify_mqtt_payload.py` simulando o pipeline fim a fim com 5 payloads (RFID, bateria e GPS).
- Files created/modified:
  - `esp32_firmware/main/simcom_ppp.h` (criado)
  - `esp32_firmware/main/simcom_ppp.c` (criado)
  - `esp32_firmware/main/mqtt_publisher.h` (criado)
  - `esp32_firmware/main/mqtt_publisher.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_mqtt_payload.py` (criado)

## Test Results (Phase 10)
| Test | Input | Expected | Actual | Status |
|------|-------|----------|--------|--------|
| Pipeline MQTT RFID YRM100 | JSON cifrado via AES-256-CBC | Decifravel com integridade | Sucesso | OK |
| Pipeline MQTT RFID WL134 | JSON cifrado via AES-256-CBC | Decifravel com integridade | Sucesso | OK |
| Pipeline MQTT Bateria | JSON cifrado via AES-256-CBC | Decifravel com integridade | Sucesso | OK |
| Pipeline MQTT GPS | JSON cifrado via AES-256-CBC | Decifravel com integridade | Sucesso | OK |

### Phase 11: Bluetooth Mobile -- GATT Server para App de Celular
- **Status:** complete
- **Started:** 2026-05-20T13:24:00
- Actions taken:
  - Criado o modulo `ble_mobile.c/.h` com servidor GATT seguro (pareamento MITM + criptografia).
  - Definidos UUIDs customizados para servico (0x00FF) e 5 caracteristicas (0xFF01-0xFF05).
  - Implementado handler de escrita para configuracao (potencia YRM100, tempo varredura, on/off leitores).
  - Implementado handler de escrita para dados de negocio (Fazenda, Lote, Animal) com persistencia NVS.
  - Implementada notificacao BLE em tempo real de tags lidas ao app conectado.
  - Implementada leitura de status do dispositivo (bateria, PPP, MQTT, Mesh, GPS, contagem de tags).
  - Integrado no dispatcher_task para notificar tag reads e atualizar bateria no status global.
  - Main loop atualizado para refresh periodico do status do dispositivo.
- Files created/modified:
  - `esp32_firmware/main/ble_mobile.h` (criado)
  - `esp32_firmware/main/ble_mobile.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_ble_gatt.py` (criado)

## Test Results (Phase 11)
| Test | Input | Expected | Actual | Status |
|------|-------|----------|--------|--------|
| UUID Uniqueness | 6 UUIDs GATT | Todos unicos | Todos unicos | OK |
| Config Write Payload | JSON config (111-113 bytes) | Formato valido < 512 bytes | Validado | OK |
| Business Data Payload | JSON farm/lot/animal (107-187 bytes) | Formato valido < 512 bytes | Validado | OK |
| Last Tag Notify | JSON tag RFID (55-65 bytes) | Formato valido com model+tag | Validado | OK |
| Device Status Read | JSON status (90-102 bytes) | Formato valido com todos campos | Validado | OK |

### Phase 12: Sistema de Cache de Dados Offline
- **Status:** complete
- **Started:** 2026-05-20T16:26:00
- **Actions taken:**
  - Criado o módulo `offline_cache.c/.h` com controle de capacidade (limite 95%), indexação sequencial persistida via SPIFFS e tarefa FreeRTOS de sincronização em background.
  - Integrado na tarefa despachante do `main.c` para salvar payloads criptografados no cache se falhar a entrega local/nuvem.
  - Modificado o `mqtt_publisher.c` para desviar mensagens destinadas à nuvem para o cache local caso o broker caia ou falhe a postagem.
- **Files created/modified:**
  - `esp32_firmware/main/offline_cache.h` (criado)
  - `esp32_firmware/main/offline_cache.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/mqtt_publisher.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_offline_cache.py` (criado)

### Phase 13: Rotina de Atualização de Firmware Remota (OTA)
- **Status:** complete
- **Started:** 2026-05-20T16:27:00
- **Actions taken:**
  - Criado o módulo `ota_manager.c/.h` integrando o componente `esp_https_ota` com lógica de verificação pós-boot e cancelamento automático de rollback se estável.
  - Implementada a seleção e priorização da interface de rede (Wi-Fi local priorizado sobre celular PPP 4G).
  - Adicionado o tratamento de comando JSON MQTT `{"cmd":"ota","url":"..."}` para acionar a atualização de firmware OTA.
- **Files created/modified:**
  - `esp32_firmware/main/ota_manager.h` (criado)
  - `esp32_firmware/main/ota_manager.c` (criado)
  - `esp32_firmware/main/mqtt_publisher.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_ota.py` (criado)

## Test Results (Phase 12, 13, 14 & 15)
| Test | Input | Expected | Actual | Status |
|------|-------|----------|--------|--------|
| FIFO Cache Sim | 4 eventos de tag/bateria | Salvamento ordenado em arquivos no cache e transmissão FIFO correta após reconexão | Executado com sucesso | OK |
| OTA JSON CMD | Payload `"cmd":"ota"` | Extração correta da URL e trigger de download | Executado com sucesso | OK |
| Redundância Rede | Conexão/Desconexão Wi-Fi | Suspensão automática do celular e reativação pelo watchdog | Executado com sucesso | OK |
| Enriquecimento Negócios | Busca de tag no banco local | JSON enriquecido com nome, peso e lote se cadastrado | Executado com sucesso | OK |

### Phase 14: Integração de Wi-Fi e Controle de Redundância de Rede
- **Status:** complete
- **Started:** 2026-05-20T17:16:00
- **Actions taken:**
  - Criado o módulo `wifi_driver.c/.h` gerenciando a interface Wi-Fi STA e conexão ao AP local (`SSID: bastaoIOT`, `Senha: 3spB@st@0`).
  - Adicionado suporte a suspensão do modem no módulo `simcom_ppp.c/.h` para liberar a sessão PPP e pausar as verificações do watchdog celular.
  - Integrado chaveamento no driver Wi-Fi: quando Wi-Fi obtém IP, o modem celular é suspenso. Quando Wi-Fi cai, o modem celular é reativado para reestabelecer a conexão celular.
  - Atualizado `main.c` para inicializar o Wi-Fi STA e gerenciar MQTT globalmente.
  - Criado script de validação de redundância e chaveamento automático de rede `teste_automatizado/verify_redundancy.py`.
- **Files created/modified:**
  - `esp32_firmware/main/wifi_driver.h` (criado)
  - `esp32_firmware/main/wifi_driver.c` (criado)
  - `esp32_firmware/main/simcom_ppp.h` (modificado)
  - `esp32_firmware/main/simcom_ppp.c` (modificado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_redundancy.py` (criado)

### Phase 15: Associação e Lógica de Negócio Local (Farm, Lot, Animal)
- **Status:** complete
- **Started:** 2026-05-20T17:23:00
- **Actions taken:**
  - Criado o módulo `animal_db.c/.h` que lê da chave NVS `biz_json` no namespace `bastao_biz` a base de cadastros de animais do curral.
  - Implementado parseamento e busca sob demanda usando a biblioteca `cJSON` para economizar memória RAM estática do ESP32.
  - Atualizada a `dispatcher_task` no `main.c` para inicializar o banco de dados no boot, aumentar o tamanho dos buffers de JSON (`json_buf` para 320, `encrypted_hex` para 768) e realizar lookup da tag lida.
  - Implementado enriquecimento do payload JSON com campos de nome do animal, peso e lote se cadastrado. Mantido payload básico como fallback de segurança.
  - Criado o script de teste de enriquecimento `teste_automatizado/verify_business_enrichment.py`.
- **Files created/modified:**
  - `esp32_firmware/main/animal_db.h` (criado)
  - `esp32_firmware/main/animal_db.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `teste_automatizado/verify_business_enrichment.py` (criado)

### Phase 16 (Refatoração): Reorganização dos Agentes de Desenvolvimento
- **Status:** complete
- **Started:** 2026-05-20T18:00:00
- **Actions taken:**
  - Unificados os agentes `stm32_agent.md` e `rfid_agent.md` em um único arquivo `stm32_firmware_agent.md`.
  - Criado o novo agente `testing_agent.md` para cobertura de testes automatizados.
  - Expandido o agente `c_best_practices_agent.md` com novas regras (header guards, limites de função, complexidade ciclomática, thread safety).
  - Removidos os arquivos obsoletos `stm32_agent.md` e `rfid_agent.md`.
  - Atualizada a documentação do projeto (`PROJETO_BASTAO.md`) com a nova estrutura de agentes.
- **Files created/modified:**
  - `.agents/stm32_firmware_agent.md` (criado - unificado)
  - `.agents/testing_agent.md` (criado)
  - `.agents/c_best_practices_agent.md` (modificado - expandido)
  - `.agents/stm32_agent.md` (removido)
  - `.agents/rfid_agent.md` (removido)
  - `PROJETO_BASTAO.md` (modificado)

### Phase 18: Tratamento de Alertas e Sinalizações Locais
- **Status:** complete
- **Started:** 2026-05-20T18:30:00
- **Actions taken:**
  - Criado o módulo `alerts.c/.h` no STM32 com detecção de bateria crítica (< 8.4V) e baixa (< 9.6V).
  - Implementado o parser de comandos JSON recebidos do ESP32 (buzzer, rfid_on/off, yrm_power, wl_power).
  - Implementado o driver de buzzer com padrões: short (100ms), long (500ms), double (150ms+100ms+150ms).
  - Adicionado pino PB6 para controle do buzzer no STM32.
  - Criado o módulo `stm32_cmd.c/.h` no ESP32 para envio de comandos ao STM32.
  - Modificada a `dispatcher_task` no ESP32 para enviar comando de buzzer short ao ler RFID e buzzer long quando bateria < 9.0V.
  - Atualizado o `stm32_uart.c` com função `stm32_uart_send_string` para envio de dados ao STM32.
- **Files created/modified:**
  - `stm32_firmware/Core/Inc/alerts.h` (criado)
  - `stm32_firmware/Core/Src/alerts.c` (criado)
  - `stm32_firmware/Core/Inc/main.h` (modificado - adicionados defines e protótipos)
  - `stm32_firmware/Core/Src/main.c` (modificado - integração de alertas e comandos)
  - `esp32_firmware/main/stm32_cmd.h` (criado)
  - `esp32_firmware/main/stm32_cmd.c` (criado)
  - `esp32_firmware/main/stm32_uart.h` (modificado - protótipo stm32_uart_send_string)
  - `esp32_firmware/main/stm32_uart.c` (modificado - implementação stm32_uart_send_string)
  - `esp32_firmware/main/main.c` (modificado - dispatcher_task com comandos de buzzer)
  - `esp32_firmware/main/CMakeLists.txt` (modificado - adicionado stm32_cmd.c)

### Phase 19: Homologação, Economia de Energia e Testes de Campo
- **Status:** in_progress
- **Started:** 2026-05-20T19:00:00
- **Actions taken:**
  - Criado módulo `power_mgmt.c/.h` no STM32 com gerenciamento de energia.
  - Implementado modo Stop (Low Power) após 30s de inatividade.
  - Wake-up automático por atividade UART (dados recebidos).
  - Criado módulo `esp_power.c/.h` no ESP32 com light sleep após 60s.
  - Criado script de teste de estresse `verify_mesh_stress.py` com 4 cenários.
- **Files created/modified:**
  - `stm32_firmware/Core/Inc/power_mgmt.h` (criado)
  - `stm32_firmware/Core/Src/power_mgmt.c` (criado)
  - `stm32_firmware/Core/Inc/main.h` (modificado - protótipos power_mgmt)
  - `stm32_firmware/Core/Src/main.c` (modificado - integração power e wake-up)
  - `esp32_firmware/main/esp_power.h` (criado)
  - `esp32_firmware/main/esp_power.c` (criado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado - adicionado esp_power)
  - `esp32_firmware/main/main.c` (modificado - integração esp_power)
   - `teste_automatizado/verify_mesh_stress.py` (criado)


## Session: 2026-06-11

### Phase 29: Refatoração do Firmware STM32 (Modularização)
- **Status:** complete
- **Actions taken:**
  - Criado `circular_buffer.c/.h` com API genérica (Init, Write, Read, Available, Peek, Flush, IsEmpty, IsFull)
  - Criado `rfid_parser.c/.h` extraindo parsing YRM100 (frame `0xBB`) e WL-134 (frame ASCII 30 bytes) do `main.c`
  - Criado `battery_monitor.c/.h` extraindo leitura ADC da bateria, com filtro de média móvel de 8 amostras
  - Limpo `main.h`: removido `BATTERY_CRITICAL_THRESHOLD 15.0f` conflitante, removidas structs/declarações movidas
  - Implementado `Command_Process()` com monitoramento de buzzer e LED de bateria crítica
  - Adicionado `Buzzer_IsActive()` ao módulo `alerts.c/.h`
  - Atualizado `main.c` para usar os novos módulos (redução de 705 → 570 linhas)
  - Adicionado heartbeat periódico do STM32 (`{"type":"heartbeat"}` a cada 30s)
- **Files created/modified:**
  - `stm32_firmware/Core/Inc/circular_buffer.h` (criado)
  - `stm32_firmware/Core/Src/circular_buffer.c` (criado)
  - `stm32_firmware/Core/Inc/rfid_parser.h` (criado)
  - `stm32_firmware/Core/Src/rfid_parser.c` (criado)
  - `stm32_firmware/Core/Inc/battery_monitor.h` (criado)
  - `stm32_firmware/Core/Src/battery_monitor.c` (criado)
  - `stm32_firmware/Core/Inc/main.h` (modificado - limpeza de defines/structs conflitantes)
  - `stm32_firmware/Core/Src/main.c` (modificado - uso dos novos módulos, heartbeat)
  - `stm32_firmware/Core/Inc/alerts.h` (modificado - add Buzzer_IsActive)
  - `stm32_firmware/Core/Src/alerts.c` (modificado - add Buzzer_IsActive)

### Phase 30: Validação e Robustez do Pipeline STM32 → ESP32
- **Status:** complete
- **Actions taken:**
  - Adicionado parser `type:"alert"` (DATA_TYPE_ALERT) e `type:"heartbeat"` (DATA_TYPE_HEARTBEAT) no stm32_uart.c
  - Removido `test_loop_enabled` e código de injeção RFID falsa do main.c do ESP32
  - Aumentado `stm32_data_queue` de 10 para 20 itens
  - Implementado watchdog STM32: `stm32_uart_is_stm32_alive()` emite alerta se >60s sem heartbeat
  - Adicionado campo `stm32_alive` na `bastao_device_status_t` exposto via BLE GATT
  - Integrado watchdog no loop principal do ESP32
- **Files created/modified:**
  - `esp32_firmware/main/stm32_uart.h` (modificado - add DATA_TYPE_ALERT, DATA_TYPE_HEARTBEAT, watchdog API)
  - `esp32_firmware/main/stm32_uart.c` (modificado - add alert/heartbeat parsers, watchdog)
  - `esp32_firmware/main/main.c` (modificado - removido test_loop, add watchdog, queue=20)
  - `esp32_firmware/main/ble_mobile.h` (modificado - add stm32_alive field)

### Phase 31: Testes de Integração e Validação Final
- **Status:** complete
- **Actions taken:**
  - Criado `verify_stm32_uart.py` com 35 testes de formato JSON, parsing, watchdog, fila
  - Criado `verify_uart_stress.py` com 13 cenarios de burst (20-100 tags), documentacao de limites
  - Criado `verify_bidirectional.py` com 22 testes de comandos ESP32->STM32 e heartbeat como ACK
  - Criado `verify_sleep_wake_uart.py` com 18 testes de sleep/wake/ciclo completo
  - Todos os 88 testes validados com sucesso (0 falhas)
- **Files created/modified:**
  - `teste_automatizado/verify_stm32_uart.py` (criado)
  - `teste_automatizado/verify_uart_stress.py` (criado)
  - `teste_automatizado/verify_bidirectional.py` (criado)
  - `teste_automatizado/verify_sleep_wake_uart.py` (criado)
  - `task_plan.md` (modificado - fase 31 concluida)
  - `ROADMAP.md` (modificado - fase 31 concluida)

## Session: 2026-06-12

### Phase 32: Reset STM32 via GPIO19 e Watchdog com Grace Period
- **Status:** complete
- **Actions taken:**
  - Adicionado `STM32_RESET_PIN GPIO_NUM_19` em `stm32_uart.h`
  - Implementado `stm32_uart_reset_init()` com correcao de glitch (gpio_set_level antes de gpio_config)
  - Implementado `stm32_uart_reset_stm32()` com pulso LOW de 100ms
  - Aplicado reset na inicializacao do ESP32 para sincronizar STM32
  - Adicionado watchdog inteligente: so conta falhas apos primeiro heartbeat recebido
  - Aumentado limiar de reset para 120 ciclos (~120s)
  - Criada funcao `stm32_uart_has_ever_been_alive()`
  - Corrigidos comentarios "GPIO23" para "GPIO19"
- **Files created/modified:**
  - `esp32_firmware/main/stm32_uart.h` (modificado - STM32_RESET_PIN, prototipos)
  - `esp32_firmware/main/stm32_uart.c` (modificado - reset_init, reset_stm32, has_ever_been_alive)
  - `esp32_firmware/main/main.c` (modificado - watchdog com grace period, comentario)

### Phase 33: Filtro de Deduplicacao de Leituras RFID
- **Status:** complete
- **Actions taken:**
  - Criado modulo `rfid_dedup.c/.h` com tabela circular de 64 entradas
  - Criterios de duplicata: mesma tag + mesma posicao GPS (<50m) + janela <10s
  - Integrado no `dispatcher_task` antes de processar RFID (continue se duplicata)
  - `rfid_dedup_init()` chamado em `app_main()`
  - Adicionado ao `CMakeLists.txt`
- **Files created/modified:**
  - `esp32_firmware/main/rfid_dedup.h` (criado)
  - `esp32_firmware/main/rfid_dedup.c` (criado)
  - `esp32_firmware/main/main.c` (modificado - include + init + dedup check)
  - `esp32_firmware/main/CMakeLists.txt` (modificado - add rfid_dedup.c)

### Phase 34: Wake Sources RFID no STM32 Sleep
- **Status:** complete
- **Actions taken:**
  - Habilitado `HAL_UARTEx_EnableStopMode(&huart3)` para WL-134
  - Habilitado `HAL_UARTEx_EnableStopMode(&huart4)` para YRM100
  - Adicionado DisableStopMode correspondente ao sair do STOP mode
- **Files created/modified:**
  - `stm32_firmware/Core/Src/power_mgmt.c` (modificado - wake sources USART3/4)

## Session: 2026-06-12 (Parte 2)

### Fase 32b: Correcao do Circuito de Reset com Transistor NPN
- **Status:** complete
- **Actions taken:**
  - Descoberto que o circuito de reset possui um **transistor NPN** entre GPIO19 e NRST, invertendo a logica
  - Corrigido `stm32_uart_reset_init()`:
    - `gpio_set_level(0)` inicial (LOW = transistor OFF = STM32 roda)
    - `GPIO_MODE_OUTPUT_OD` → `GPIO_MODE_OUTPUT_PP` (push-pull para acionar transistor)
    - Removido pull-up (nao necessario para push-pull)
  - Corrigido `stm32_uart_reset_stm32()`:
    - Antes: LOW → delay → HIGH (logica invertida)
    - Depois: HIGH → delay → LOW (HIGH = transistor ON = NRST LOW = reset)
  - Atualizada documentacao em `PROJETO_BASTAO.md`, `stm32_uart.h`, `stm32_uart.c`
  - Atualizado `ROADMAP.md` com a correcao do circuito
- **Files created/modified:**
  - `esp32_firmware/main/stm32_uart.c` (modificado - logica invertida e push-pull)
  - `esp32_firmware/main/stm32_uart.h` (modificado - documentacao do transistor)
  - `PROJETO_BASTAO.md` (modificado - secao do circuito de reset)
  - `ROADMAP.md` (modificado - task 32.8)
  - `progress.md` (modificado - esta sessao)

## Session: 2026-06-12 (Parte 3)

### Fase 35: Correcao do Circuito de Reset com Transistor NPN
- **Status:** complete
- **Actions taken:**
  - Descoberto transistor NPN entre GPIO19 e NRST (logica invertida)
  - Alterado modo de `OUTPUT_OD` para `OUTPUT_PP` (push-pull)
  - `reset_init`: inicializa LOW (STM32 roda)
  - `reset_stm32`: HIGH 100ms → LOW (corrigido)
- **Files modified:**
  - `esp32_firmware/main/stm32_uart.c` (modificado)
  - `esp32_firmware/main/stm32_uart.h` (modificado)

### Fase 36: Filtro de Deduplicacao RFID
- **Status:** complete
- **Actions taken:**
  - Criado `rfid_dedup.c/.h` com tabela circular de 64 entradas
  - Criterios: tag + modelo + 10s + GPS <50m
  - Integrado no `dispatcher_task` no main.c
- **Files created/modified:**
  - `esp32_firmware/main/rfid_dedup.h` (criado)
  - `esp32_firmware/main/rfid_dedup.c` (criado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)

### Fase 37: Gerenciamento de Chip SIM (DSSS)
- **Status:** complete
- **Actions taken:**
  - Substituido `AT+SWITCHSIM` por `AT*SELECTSIMSLOT` (com * proprietario)
  - Sequencia DSSS: CFUN=0 → SELECTSIMSLOT → CFUN=1 → CPIN?
  - Removidos fallbacks SWITCHSIM/DUALSIM
  - Corrigido CCID: `AT+CCID` → `AT+CICCID`
  - Watchdog ajustado para swap so se chip ausente
- **Files modified:**
  - `esp32_firmware/main/simcom_ppp.c` (modificado)
  - `esp32_firmware/main/simcom_ppp.h` (modificado)

### Fase 38: Sincronizacao de Horario (Torre + SNTP)
- **Status:** complete
- **Actions taken:**
  - Criado `simcom_ppp_sync_time_from_tower()` com AT+CCLK e settimeofday
  - Adicionado SNTP com servidores pool.ntp.org, a.ntp.br, b.ntp.br
  - Log [HB] a cada 10s com horario formatado
  - Log [MQTT_ENVIO] com payload pre-criptografia
- **Files modified:**
  - `esp32_firmware/main/simcom_ppp.c` (modificado)
  - `esp32_firmware/main/simcom_ppp.h` (modificado)
  - `esp32_firmware/main/main.c` (modificado)

## Session: 2026-06-12 (Parte 4 - Final)

### Fase 39: Correcao Sleep STM32 e Wake via UART
- **Status:** complete
- **Actions taken:**
  - POWER_SLEEP_TIMEOUT_MS: 30s → 120s
  - Log JSON sleep/wake do STM32 para ESP32
  - Re-arm RX interrupts apos wake
  - ESP32 envia \n apos 60s sem heartbeat como wake preventivo
  - DATA_TYPE_POWER adicionado no parser
- **Files modified:**
  - `stm32_firmware/Core/Inc/power_mgmt.h` (modificado)
  - `stm32_firmware/Core/Src/power_mgmt.c` (modificado)
  - `esp32_firmware/main/stm32_uart.c` (modificado - DATA_TYPE_POWER)

### Fase 40: Otimizacao UART e Core Pinning
- **Status:** complete
- **Actions taken:**
  - UART RX alterado de byte-a-byte para leitura em blocos de 1024 bytes
  - Todas as tarefas FreeRTOS fixadas em nucleos dedicados:
    - Core 0: stm32_uart_rx (prio 7), dispatcher (prio 6)
    - Core 1: watchdog, MQTT, PPP, OTA, cache, logger
- **Files modified:**
  - `esp32_firmware/main/stm32_uart.c` (modificado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/simcom_ppp.c` (modificado)
  - `esp32_firmware/main/mqtt_publisher.c` (modificado)
  - `esp32_firmware/main/offline_cache.c` (modificado)
  - `esp32_firmware/main/esp32_logger.c` (modificado)
  - `esp32_firmware/main/ota_manager.c` (modificado)

### Fase 41: GPS Logs de Diagnostico
- **Status:** complete
- **Actions taken:**
  - Todos os logs de GPS alterados para ESP_LOGI (visiveis sempre)
  - Logs detalhados: cold start, tentativas, parse, fix
  - Prefixo [GPS] em todas as mensagens
- **Files modified:**
  - `esp32_firmware/main/simcom_ppp.c` (modificado)

## Session: 2026-06-15

### Fase 42: Transição de Conectividade Celular (Remoção do PPP e Uso de Comandos AT + MQTT Direto)
- **Status:** complete
- **Actions taken:**
  - Removido completamente o protocolo PPP sobre interface celular (desativado `esp_netif_ppp` e pilha de rede celular LwIP).
  - Criado o novo driver `simcom_driver.c/.h` contendo parser serial avançado (`simcom_uart_rx_task`), sincronização por mutexes e semáforos, processador de URCs (incluindo `+CMQTTDELIVER` e `+CMQTTCONNLOST`), e cliente MQTT nativo embutido do modem SIMCom.
  - Excluído os arquivos obsoletos `simcom_ppp.c` e `simcom_ppp.h`.
  - Refatorado `mqtt_publisher.c/.h` para direcionar de forma dinâmica o envio via `esp_mqtt` (se por Wi-Fi) ou `simcom_driver_mqtt_publish` (se por 4G Celular).
  - Atualizado `ota_manager.c` para restringir atualizações OTA exclusivamente a conexões Wi-Fi, já que a rede celular não possui mais roteamento LwIP local.
  - Sincronização de relógio (SNTP) restrita a Wi-Fi; no celular o tempo é sincronizado diretamente da torre via comando `AT+CCLK?`.
  - Atualizado `main.c`, `wifi_driver.c`, `ble_mobile.c` e documentações técnicas (`ROADMAP.md`, `Manual/arquitetura.md`, `Manual/funcionalidades.md`, `task_plan.md`, `PROJETO_BASTAO.md` e `README.md`).
- **Files created/modified/deleted:**
  - `esp32_firmware/main/simcom_driver.h` (criado)
  - `esp32_firmware/main/simcom_driver.c` (criado)
  - `esp32_firmware/main/simcom_ppp.h` (deletado)
  - `esp32_firmware/main/simcom_ppp.c` (deletado)
  - `esp32_firmware/main/CMakeLists.txt` (modificado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/mqtt_publisher.h` (modificado)
  - `esp32_firmware/main/mqtt_publisher.c` (modificado)
  - `esp32_firmware/main/wifi_driver.c` (modificado)
  - `esp32_firmware/main/ble_mobile.c` (modificado)
  - `esp32_firmware/main/ota_manager.c` (modificado)
  - `Manual/arquitetura.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `ROADMAP.md` (modificado)
  - `task_plan.md` (modificado)
  - `PROJETO_BASTAO.md` (modificado)
  - `README.md` (modificado)
  - `progress.md` (modificado)


## Session: 2026-06-18

### Phase 45: Otimização SIMCom (Captura de Tags) + GPS Hot Start
- **Status:** complete
- **Started:** 2026-06-18T08:56:00
- **Actions taken:**
  - Desacoplado o envio MQTT em modo `CELLULAR_ONLY` do loop do despachante. A fila e a tarefa `mqtt_pub_task` agora rodam de forma incondicional, enviando no background e evitando perdas de tags por latência serial do modem.
  - Corrigido bug no watchdog que recriava sucessivamente a task `simcom_rx` e deletava o driver UART. Agora são configurados apenas uma vez.
  - Implementada a flag `first_init_done` para pular a varredura DSSS física de slots SIM no watchdog, cortando tempo de inicialização em 15s.
  - Otimizado `simcom_driver_get_status` e o orquestrador para lerem métricas em cache. Operações lentas de leitura de operadora (`AT+COPS?`) e logs de erro (`AT+CEER`) agora ocorrem em segundo plano a cada 5 minutos.
  - Configurado GPS em modo Hot Start rápido via AP_Flash usando `AT+CGNSSPWR=1,1` (power on) e `AT+CGNSSPWR=0,1` (power off) para armazenar órbitas.
- **Files modified:**
  - `esp32_firmware/main/mqtt_publisher.c` (modificado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/main/simcom_driver.c` (modificado)
  - `ROADMAP.md` (modificado)
  - `task_plan.md` (modificado)
   - `AGENTS.md` (modificado)
   - `progress.md` (modificado)



## Session: 2026-06-25

### Phase 47: Documentação Completa de OTA
- **Status:** complete
- **Actions taken:**
  - Mapeada capacidade OTA atual do projeto: ESP32 (Wi-Fi apenas ✅), K10 (não suporta ❌), modem SIMCom (FOTA não implementado ❌).
  - Adicionada seção "Atualização OTA" completa em `PROJETO_BASTAO.md` — arquitetura, partições, fluxo, limitações, geração de binário, teste.
  - Corrigida seção 2.6 em `Manual/funcionalidades.md` — removida menção incorreta a "4G", adicionadas limitações reais, esclarecido escopo (ESP32 apenas).
  - Atualizada `Manual/arquitetura.md` — adicionadas notas sobre partições A/B e rollback.
  - Adicionadas 3 fases futuras em `ROADMAP.md` (Fase 44: OTA via 4G, Fase 45: OTA na K10, Fase 46: CI/CD + Report).
  - Adicionada seção de preparação de binário OTA em `COMPILATION_GUIDE.md`.
  - Adicionados indicadores de limitação OTA no `README.md`.
  - Sessão 35 registrada em `AGENTS.md`.
- **Files modified:**
  - `PROJETO_BASTAO.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `Manual/arquitetura.md` (modificado)
  - `ROADMAP.md` (modificado)
  - `COMPILATION_GUIDE.md` (modificado)
  - `README.md` (modificado)
  - `AGENTS.md` (modificado)
  - `progress.md` (modificado)

## Session: 2026-07-02

### Phase 66.1: Buzzer via I2S (Speaker NS4168)
- **Status:** complete
- **Actions taken:**
  - Criado `k10_hal/hal_buzzer.c/.h` com driver I2S usando `driver/i2s_std.h` (API nova ESP-IDF v5.4.4).
  - Pinos: BCLK=GPIO0, LRCK=GPIO38, SDO=GPIO45 (MCLK=NC).
  - Geração de sine wave 44100Hz/16-bit/mono.
  - Tipos de beep: short(100ms), long(300ms), double, alert(2kHz).
  - Incluído `FreeRTOS.h` para corrigir `xTaskCreatePinnedToCore` não declarado.
- **Files created:**
  - `k10_firmware/components/k10_hal/hal_buzzer.c` (criado)
  - `k10_firmware/components/k10_hal/hal_buzzer.h` (criado)
- **Files modified:**
  - `k10_firmware/components/k10_hal/CMakeLists.txt` (modificado)
  - `k10_firmware/main/main.c` (modificado)

### Phase 66.2: Tela "BRINCO LIDO"
- **Status:** complete
- **Actions taken:**
  - Tela branca fullscreen com "BRINCO LIDO" + número da tag.
  - Auto-dismiss após 3s via `lv_timer_create()`.
  - Texto "GPS SINCRONIZADO" exibido abaixo de lat/lon quando fix=true.
  - Corrigido `lv_font_montserrat_28` → `lv_font_montserrat_24`.
  - Habilitado `CONFIG_LV_FONT_MONTSERRAT_20=y` e `CONFIG_LV_FONT_MONTSERRAT_24=y` em sdkconfig.
- **Files modified:**
  - `k10_firmware/components/gui/gui_manager.c` (modificado)
  - `k10_firmware/components/gui/include/gui_manager.h` (modificado)
  - `k10_firmware/sdkconfig.defaults` (modificado)
  - `k10_firmware/sdkconfig` (modificado)

### Phase 66.3: SPIFFS + Contador Diário
- **Status:** complete
- **Actions taken:**
  - Novo componente `tag_database/` com `tag_database.c/.h`.
  - Partição SPIFFS (2MB em 0x310000) adicionada ao `partitions.csv`.
  - Arquivos JSON por data (`/spiffs/tags/YYYY-MM-DD.json`).
  - Buffer em RAM com auto-save a cada 10 reads.
  - Flash K10 atualizada 4MB→16MB em sdkconfig.
- **Files created:**
  - `k10_firmware/components/tag_database/tag_database.c` (criado)
  - `k10_firmware/components/tag_database/tag_database.h` (criado)
  - `k10_firmware/components/tag_database/CMakeLists.txt` (criado)
- **Files modified:**
  - `k10_firmware/components/gui/CMakeLists.txt` (modificado)
  - `k10_firmware/main/CMakeLists.txt` (modificado)
  - `k10_firmware/partitions.csv` (modificado)
  - `k10_firmware/sdkconfig.defaults` (modificado)
  - `k10_firmware/sdkconfig` (modificado)

### Phase 66.4: Aba Histórico (Tab2)
- **Status:** complete
- **Actions taken:**
  - Reescrito `create_screen_connectivity()` → `create_screen_history()`.
  - Header com ícone olho + total de leituras + tags únicas.
  - Lista scrollável com últimas 20 tags (tag ID + nome animal).
  - `gui_manager_refresh_history()` chamado após cada leitura RFID.
  - Ícone da aba mudado de WiFi→LIST.
- **Files modified:**
  - `k10_firmware/components/gui/gui_manager.c` (modificado)
  - `k10_firmware/components/gui/include/gui_manager.h` (modificado)

### Phase 66.5: GPS Gate + Triangulação Celular
- **Status:** complete
- **Actions taken:**
  - Extraído TAC/CID/EARFCN de `AT+CPSI?` em `query_cpsi_metrics()`.
  - Adicionada `cell_tower_get_location()` via Mozilla Location Service HTTPS POST.
  - Adicionadas `simcom_driver_has_location()`, `simcom_driver_get_cell_tower_location()`, `simcom_driver_is_cell_tower_valid()`.
  - Dispatcher: GPS优先 → cell tower fallback → sem localização: tag descartada do MQTT.
  - Atualização de partições: ESP32 OTA 4MB cada, SPIFFS 2MB, flash 16MB.
- **Files modified:**
  - `esp32_firmware/main/simcom_driver.c` (modificado)
  - `esp32_firmware/main/simcom_driver.h` (modificado)
  - `esp32_firmware/main/main.c` (modificado)
  - `esp32_firmware/partitions.csv` (modificado)
  - `esp32_firmware/sdkconfig.defaults` (modificado)
  - `esp32_firmware/sdkconfig` (modificado)

### Phase 66.6: Documentação Atualizada
- **Status:** complete
- **Actions taken:**
  - Atualizado `AGENTS.md` com Sessão 20.
  - Atualizado `PROJETO_BASTAO.md` com seção K10.
  - Atualizado `README.md` com novas features.
  - Atualizado `ROADMAP.md` com fases 66.1-66.5.
  - Atualizado `Manual/funcionalidades.md` com funcionalidades K10.
  - Atualizado `Manual/arquitetura.md` com pinagem K10.
  - Atualizado `documentacao/01_Arquitetura_do_Sistema.md` com GPS gate.
  - Atualizado `documentacao/08_Roadmap.md` com K10 features.
- **Files modified:**
  - `AGENTS.md` (modificado)
  - `PROJETO_BASTAO.md` (modificado)
  - `README.md` (modificado)
  - `ROADMAP.md` (modificado)
  - `Manual/funcionalidades.md` (modificado)
  - `Manual/arquitetura.md` (modificado)
  - `documentacao/01_Arquitetura_do_Sistema.md` (modificado)
  - `documentacao/08_Roadmap.md` (modificado)
  - `progress.md` (modificado)


