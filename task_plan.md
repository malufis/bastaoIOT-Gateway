# Plano de Tarefas: Implantação e Estruturação do Bastão-ESP

## Goal
Estruturar o projeto Bastão-ESP, criando a estrutura física de diretórios, documentação de manuais humanos, base de conhecimento de protocolos RFID e perfis de agentes, e implementando o firmware completo dos microcontroladores STM32 e ESP32 com conectividade celular, Wi-Fi, redundância, cache offline, criptografia de dados, suporte a BLE GATT e OTA.

## Current Phase
Phase 16: Protocolo de Comandos Remotos via MQTT e BLE

## Phases

### Phase 1: Estruturação Física e Diretórios
- [x] Criar a pasta `Manual/`
- [x] Criar a pasta `teste_automatizado/` com `.gitkeep`
- [x] Criar a pasta `debug/` com `.gitkeep`
- [x] Criar a pasta `aprendizado/`
- **Status:** complete

### Phase 2: Documentação de Manuais Humanos
- [x] Criar `Manual/git_guidelines.md` com as regras de Git e versionamento
- [x] Criar `Manual/arquitetura.md` com o mapeamento físico de pinos e fluxos de dados
- [x] Criar `Manual/funcionalidades.md` com as descrições de lógicas de código do STM32 e ESP32
- **Status:** complete

### Phase 3: Base de Conhecimento de Protocolos (aprendizado)
- [x] Criar `aprendizado/rfid_protocols.md` compilando a especificação detalhada de decodificação do YRM100 e WL-134 para IAs
- **Status:** complete

### Phase 4: Atualização e Criação dos Perfis de Agente
- [x] Atualizar `.agents/rfid_agent.md` com os detalhes matemáticos e decodificações
- [x] Criar `.agents/stm32_agent.md` configurado para a MCU STM32G070CBTx e suas conexões
- **Status:** complete

### Phase 5: Atualização do README Geral
- [x] Atualizar o `README.md` raiz com uma visão geral direta do hardware e frameworks do projeto
- **Status:** complete

### Phase 6: Comunicação Bluetooth no ESP32
- [x] Adicionar detalhes de comunicação Bluetooth com app de celular em `Manual/funcionalidades.md`
- **Status:** complete

### Phase 7: Atualização Remota (OTA) no ESP32
- [x] Adicionar detalhes de atualização remota (OTA) via Wi-Fi e 4G em `Manual/funcionalidades.md`
- **Status:** complete

### Phase 8: Sistema de Cache de Dados Offline
- [x] Adicionar detalhes de armazenamento offline e sincronização posterior em `Manual/funcionalidades.md`
- **Status:** complete

### Phase 9: Criptografia de Payload e Despachante ESP32
- [x] Criar o módulo `secure_payload.c/.h` com criptografia AES-256-CBC
- [x] Criar o esqueleto do módulo `mesh_coordinator.c/.h` para rede BLE Mesh
- [x] Implementar a tarefa despachante (`dispatcher_task`) no `main.c` do ESP32
- [x] Adicionar script de teste `teste_automatizado/verify_encryption.py`
- **Status:** complete

### Phase 10: SIMCom 7663E -- Driver Celular AT-MQTT + GPS
- [x] Criar o módulo `simcom_driver.c/.h` com comandos AT e watchdog celular
- [x] Criar o módulo `mqtt_publisher.c/.h` com cliente MQTT e chaveamento de rotas
- [x] Criar script de teste `teste_automatizado/verify_mqtt_payload.py`
- **Status:** complete

### Phase 11: Bluetooth Mobile -- GATT Server para App de Celular
- [x] Criar o módulo `ble_mobile.c/.h` com GATT Server e UUIDs customizados
- [x] Implementar persistência NVS para configurações e dados de negócio
- [x] Criar script de teste `teste_automatizado/verify_ble_gatt.py`
- **Status:** complete

### Phase 12: Sistema de Cache de Dados Offline (SPIFFS FIFO)
- [x] Criar o módulo `offline_cache.c/.h` com montagem SPIFFS e limites de segurança
- [x] Desenvolver a fila FIFO baseada em arquivos sequenciais na Flash local
- [x] Implementar a tarefa de sincronização automática e descarte de cache pós-ACK MQTT
- [x] Criar script de teste `teste_automatizado/verify_offline_cache.py`
- **Status:** complete

### Phase 13: Rotina de Atualização de Firmware Remota (OTA HTTPS)
- [x] Criar o módulo `ota_manager.c/.h` com `esp_https_ota` e rollback seguro
- [x] Implementar inscrição e tratamento de comandos MQTT no tópico `bastao/cmd`
- [x] Criar script de teste `teste_automatizado/verify_ota.py`
- **Status:** complete

### Phase 14: Integração de Wi-Fi e Controle de Redundância de Rede
- [x] Criar o módulo `wifi_driver.c/.h` para Wi-Fi Station
- [x] Implementar lógica de suspensão do modem celular (`simcom_driver_set_suspended`)
- [x] Integrar comutador automático de rota: suspende celular no Wi-Fi IP, reativa quando Wi-Fi cai
- [x] Criar script de teste `teste_automatizado/verify_redundancy.py`
- **Status:** complete

### Phase 15: Associação e Lógica de Negócio Local (Farm, Lot, Animal)
- [x] Criar o módulo de banco de dados de animais local `animal_db.c/.h`
- [x] Implementar carregamento e consulta sob demanda via `cJSON` a partir da NVS
- [x] Integrar no despachante (`dispatcher_task`) do ESP32 para gerar JSON enriquecido
- [x] Criar script de teste `teste_automatizado/verify_business_enrichment.py`
- **Status:** complete

### Phase 16: Protocolo de Comandos Remotos via MQTT e BLE
- [x] Estender o parser JSON do comando recebido em `bastao/cmd`
- [x] Implementar alteração dinâmica de configurações via comandos remotos
- [x] Roteamento de comandos de hardware do ESP32 para o STM32 via serial UART
- **Status:** complete

### Phase 17: Refatoração dos Agentes de Desenvolvimento (Concluído)
- [x] Unificar `stm32_agent.md` e `rfid_agent.md` em `stm32_firmware_agent.md`
- [x] Criar `testing_agent.md` para testes automatizados
- [x] Expandir `c_best_practices_agent.md` com novas regras
- [x] Remover arquivos obsoletos (`stm32_agent.md`, `rfid_agent.md`)
- [x] Atualizar documentação do projeto (`PROJETO_BASTAO.md`)
- **Status:** complete

### Phase 18: Tratamento de Alertas e Sinalizações Locais
- [x] Implementar sistema de alertas de bateria no STM32 (crítico < 8.4V)
- [x] Adicionar comando de buzzer no protocolo ESP32->STM32
- [x] Criar parser de comandos do ESP32 no STM32
- [x] Implementar acionamento de buzzer via GPIO no STM32 (PB6)
- [x] Atualizar firmware ESP32 para enviar comandos de alerta
- **Status:** complete

### Phase 19: Homologação, Economia de Energia e Testes de Campo
- [x] Implementar modo Sleep no STM32 quando inativo (30s timeout)
- [x] Implementar modo Sleep no ESP32 (light sleep 60s timeout)
- [x] Criar lógica de wake-up por atividade RFID/UART
- [x] Criar script de teste de estresse Mesh (`verify_mesh_stress.py`)
- **Status:** complete

### Phase 20: Integração Acelerômetro K10 → Bastão-ESP
- [x] Atualizar protocolo de comunicação para incluir acelerômetro
- [x] Modificar K10 para enviar dados do acelerômetro via Mesh
- [x] Modificar Bastão-ESP para processar dados do acelerômetro
- [x] Adicionar validação de movimento na leitura RFID
- **Status:** complete

### Phase 21: Configuração de Rede via App Mobile (NOVA)
- [x] Adicionar campos Wi-Fi (SSID/Senha) ao GATT Server
- [x] Adicionar campos APN ao GATT Server
- [x] Adicionar campos MQTT ao GATT Server
- [x] Implementar apply de configuração (reiniciei componentes)
- [x] Criar validação de entrada de dados
- **Status:** complete

### Phase 22: Monitoramento de Rede Celular (NOVA)
- [x] Implementar leitura de CSQ (qualidade de sinal)
- [x] Implementar detecção de tecnologia (4G/LTE/NB-IoT)
- [x] Implementar captura de erros detalhados da rede celular
- [x] Adicionar característica GATT 0xFF06 para diagnóstico
- **Status:** complete

### Phase 23: Modos de Baixo Consumo (Sleep Modes) (NOVA)
- [x] Implementar Deep Sleep no STM32
- [x] Implementar Light/Deep Sleep no ESP32
- [x] Adicionar wake-up por timer RTC
- [x] Integrar wake-up com atividade RFID
- [x] Criar testes de autonomia de bateria
- **Status:** complete

### Phase 24: Testes de Estresse em Campo - Rede Mesh K10 (NOVA)
- [x] Executar testes de flood e perda de pacotes
- [x] Validar reconexao apos desconexao
- [x] Testar interferencia Wi-Fi/BLE
- [x] Implementar teste de latencia
- [x] Documentar recomendacoes para campo
- **Status:** complete

### Phase 25: Debug Wireless via Telnet/BLE (NOVA)
- [x] Criar modulo esp32_logger.c com Telnet server
- [x] Criar modulo esp32_logger.h com API de logging
- [x] Adicionar caracteristica GATT 0xFF07 para log
- [x] Integrar com BLE Mobile para notificacoes
- [x] Criar script Python esp32_log_viewer.py
- [x] Inicializar logger no boot do main.c
- **Status:** complete

### Phase 26: SMS, Logs de Antena e Subscrições de Configuração (NOVA)
- [x] Implementar subscrição de tópicos de configuração remota via MQTT (`id/<ID>/config`)
- [x] Implementar leitura de erros celular estendidos via comando AT `AT+CEER` e diagnóstico de antena
- [x] Implementar engine de envio e recepção de SMS para contingência/alertas
- **Status:** complete

### Phase 29: Refatoração do Firmware STM32 (Modularização)
- [x] 29.1 — Criar `rfid_parser.c/.h` com parsing YRM100 e WL-134
- [x] 29.2 — Criar `battery_monitor.c/.h` com ADC e filtro de média móvel
- [x] 29.3 — Criar `circular_buffer.c/.h` com buffer circular genérico
- [x] 29.4 — Limpar `main.h` (remover BATTERY_CRITICAL_THRESHOLD conflitante)
- [x] 29.5 — Implementar `Command_Process()` com monitoramento de buzzer/LED
- [x] 29.6 — Substituir buffers manuais por módulo `circular_buffer` e `rfid_parser`
- **Status:** complete

### Phase 30: Validação e Robustez do Pipeline STM32 → ESP32
- [x] 30.1 — Adicionar parser `type:"alert"` no `stm32_uart.c` do ESP32
- [x] 30.2 — Remover `test_loop_enabled` e código de injeção RFID falsa do ESP32
- [x] 30.3 — Aumentar `stm32_data_queue` de 10 para 20+ itens
- [x] 30.4 — Adicionar heartbeat periódico do STM32 (`{"type":"heartbeat"}` a cada 30s)
- [x] 30.5 — Watchdog de timeout STM32 no ESP32 (alerta se >60s sem heartbeat)
- **Status:** complete

### Phase 31: Testes de Integração e Validação Final
- [x] 31.1 — Criar `verify_stm32_uart.py` (simula dados do STM32, 35 testes)
- [x] 31.2 — Criar `verify_uart_stress.py` (burst de 20-50 tags, 13 cenarios)
- [x] 31.3 — Criar `verify_bidirectional.py` (9 comandos + heartbeat, 22 testes)
- [x] 31.4 — Criar `verify_sleep_wake_uart.py` (sleep/wake/ ciclos, 18 testes)
- **Status:** complete
- **Resultado:** 88/88 testes passaram (0 falhas)

### Phase 32: Reset STM32 via GPIO19 e Watchdog com Grace Period
- [x] Adicionar pino STM32_RESET_PIN (GPIO19) em stm32_uart.h
- [x] Implementar stm32_uart_reset_init() com gpio_set_level antes de gpio_config (evitar glitch)
- [x] Implementar stm32_uart_reset_stm32() com pulso LOW 100ms
- [x] Aplicar reset na inicializacao do ESP32 (500ms delay + 100ms pulso)
- [x] Adicionar stm32_uart_has_ever_been_alive() para grace period
- [x] Watchdog: so conta falhas apos primeiro heartbeat; limiar 120 ciclos
- [x] Corrigir comentarios GPIO23 para GPIO19
- [x] **Circuito com transistor NPN:** GPIO19 → transistor → NRST (logica invertida)
  - [x] `reset_init`: inicial em LOW (transistor OFF, NRST HIGH, STM32 roda)
  - [x] `reset_init`: modo Push-Pull (Open-Drain nao acionava transistor corretamente)
  - [x] `reset_stm32`: HIGH 100ms → LOW (inverter logica)
  - [x] Documentar circuito em PROJETO_BASTAO.md
- **Status:** complete

### Phase 33: Filtro de Deduplicacao de Leituras RFID
- [x] Criar modulo rfid_dedup.c/.h com tabela circular de 64 entradas
- [x] Implementar rfid_dedup_is_duplicate(tag, model, lat, lon, has_gps)
- [x] Criterios: mesma tag + 10s janela + GPS < 50m (se disponivel)
- [x] Integrar no dispatcher_task antes do processamento RFID
- [x] Adicionar rfid_dedup_init() no app_main()
- [x] Adicionar rfid_dedup.c ao CMakeLists.txt
- **Status:** complete

### Phase 34: Wake Sources RFID no STM32 Sleep
- [x] Habilitar HAL_UARTEx_EnableStopMode(&huart3) para WL-134
- [x] Habilitar HAL_UARTEx_EnableStopMode(&huart4) para YRM100
- [x] Adicionar DisableStopMode correspondente ao sair do STOP mode
- **Status:** complete

### Phase 35: Correcao do Circuito de Reset com Transistor NPN
- [x] Descobrir transistor NPN entre GPIO19 e NRST (logica invertida)
- [x] Alterar modo de OUTPUT_OD para OUTPUT_PP (push-pull)
- [x] reset_init: inicializar LOW (transistor OFF, STM32 roda)
- [x] reset_stm32: HIGH 100ms (reset) → LOW (roda)
- **Status:** complete

### Phase 36: Filtro de Deduplicacao RFID
- [x] Criar modulo rfid_dedup.c/.h com tabela circular de 64 entradas
- [x] Criterios: mesma tag + modelo + 10s janela + GPS <50m
- [x] Integrar rfid_dedup_is_duplicate() no dispatcher_task
- [x] Adicionar ao CMakeLists.txt
- **Status:** complete

### Phase 37: Gerenciamento de Chip SIM (DSSS)
- [x] Substituir AT+SWITCHSIM por AT*SELECTSIMSLOT (comando proprietario)
- [x] Implementar sequencia DSSS: CFUN=0 → SELECTSIMSLOT → CFUN=1 → CPIN?
- [x] Remover fallbacks SWITCHSIM/DUALSIM (outra serie)
- [x] Corrigir CCID: AT+CCID → AT+CICCID
- [x] Watchdog: swap so se chip ausente
- **Status:** complete

### Phase 38: Sincronizacao de Horario (Torre + SNTP)
- [x] Criar simcom_driver_sync_time_from_tower() com AT+CCLK
- [x] Inicializar SNTP com pool.ntp.org, a.ntp.br, b.ntp.br
- [x] Adicionar log [HB] a cada 10s com horario formatado
- [x] Adicionar log [MQTT_ENVIO] com payload pre-criptografia
- **Status:** complete

### Phase 39: Correcao Sleep STM32 e Wake via UART
- [x] Aumentar POWER_SLEEP_TIMEOUT_MS para 120s
- [x] Adicionar log JSON sleep/wake no STM32
- [x] Re-armar UART RX interrupts apos wake
- [x] ESP32 enviar \n como wake preventivo apos 60s
- [x] Adicionar DATA_TYPE_POWER no parser do ESP32
- **Status:** complete

### Phase 40: Otimizacao UART (Blocos) e Core Pinning
- [x] Alterar uart_read_bytes para ler blocos de ate 1024 bytes
- [x] Fixar stm32_uart_rx_task e dispatcher_task no Core 0
- [x] Fixar watchdog, MQTT, celular, OTA, cache, logger no Core 1
- **Status:** complete

### Phase 41: GPS Logs de Diagnostico
- [x] Alterar logs GPS para ESP_LOGI (sempre visiveis)
- [x] Adicionar logs de cold start, tentativas, parse e fix
- [x] Adicionar prefixo [GPS] em todas as mensagens
- **Status:** complete

### Phase 42: Transição de PPP para Comandos AT Diretos (SIMCom MQTT)
- [x] Criar novo driver `simcom_driver.c/.h` com comandos AT diretos
- [x] Remover o uso da pilha PPP e `esp_netif_ppp`
- [x] Implementar chaveamento dinâmico no publicador MQTT
- [x] Atualizar a orquestração do OTA para rodar exclusivamente em Wi-Fi
- [x] Atualizar documentações e roadmaps do projeto
- **Status:** complete

### Phase 45: Otimização SIMCom (Captura de Tags) + GPS Hot Start
- [x] Desacoplar envio MQTT no modo CELLULAR_ONLY via fila/task dedicada
- [x] Evitar recriação de task UART e reinstalação de driver no watchdog
- [x] Ignorar sondagem física DSSS redundante de slots usando first_init_done
- [x] Consultar status de sinal e tecnologia a partir do cache na leitura periódica
- [x] Habilitar Hot Start de GPS enviando AT+CGNSSPWR=1,1 e AT+CGNSSPWR=0,1
- **Status:** complete

## Tarefas Pendentes
- **Nenhuma. Todas as fases 1-45 concluidas.**
- **Status:** complete

## Key Questions
1. Como o STM32 envia a leitura de bateria? (JSON via UART2)
2. Qual a pinagem exata e a configuração serial de cada leitor? (YRM100: USART4 PA0/PA1 115200 8N1; WL-134: USART3 PA5/PB0 9600 8N2)

## Decisions Made
| Decision | Rationale |
|----------|-----------|
| Banco de Dados NVS | Dados de negócios (Farm/Lot/Animal) persistidos na NVS com busca em array JSON dinâmico sob demanda para economizar RAM |
| Redundância de Rede | Conexão de dados Wi-Fi STA priorizada sobre celular 4G para economizar custos e consumo. Suspensão celular automática enquanto Wi-Fi possui IP válido. |

## Errors Encountered
| Error | Attempt | Resolution |
|-------|---------|------------|
| Falha build idf.py local | 1 | Comando idf.py indisponível no path do sistema local. Validação lógica feita com simuladores Python. |

## Notes
- Atualizar o status de cada fase conforme avançar no desenvolvimento.
- Manter o checklist de tarefas atualizado para acompanhamento no Git.
