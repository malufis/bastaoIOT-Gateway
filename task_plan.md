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

### Phase 10: SIMCom 7663E -- Driver Celular PPP + MQTT
- [x] Criar o módulo `simcom_ppp.c/.h` com comandos AT, PPP e watchdog celular
- [x] Criar o módulo `mqtt_publisher.c/.h` com cliente MQTT nativo
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
- [x] Implementar lógica de suspensão do modem celular PPP (`simcom_ppp_set_suspended`)
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
- [ ] Estender o parser JSON do comando recebido em `bastao/cmd`
- [ ] Implementar alteração dinâmica de configurações via comandos remotos
- [ ] Roteamento de comandos de hardware do ESP32 para o STM32 via serial UART
- **Status:** pending

### Phase 17: Refatoração dos Agentes de Desenvolvimento (Concluído)
- [x] Unificar `stm32_agent.md` e `rfid_agent.md` em `stm32_firmware_agent.md`
- [x] Criar `testing_agent.md` para testes automatizados
- [x] Expandir `c_best_practices_agent.md` com novas regras
- [x] Remover arquivos obsoletos (`stm32_agent.md`, `rfid_agent.md`)
- [x] Atualizar documentação do projeto (`PROJETO_BASTAO.md`)
- **Status:** complete

### Phase 18: Tratamento de Alertas e Sinalizações Locais
- [ ] Implementar medição e envio de bateria no STM32 com alertas de nível crítico (< 15%)
- [ ] Desenvolver acionamento de feedback sonoro (Buzzer) no STM32 via comandos do ESP32
- **Status:** pending

### Phase 19: Homologação, Economia de Energia e Testes de Campo
- [ ] Implementar modos de baixo consumo (Sleep Modes) no STM32 e ESP32
- [ ] Validar testes de estresse em campo de perdas de pacote na rede Mesh com a Tela K10
- **Status:** pending

## Key Questions
1. Como o STM32 envia a leitura de bateria? (JSON via UART2)
2. Qual a pinagem exata e a configuração serial de cada leitor? (YRM100: USART4 PA0/PA1 115200 8N1; WL-134: USART3 PA5/PB0 9600 8N2)

## Decisions Made
| Decision | Rationale |
|----------|-----------|
| Banco de Dados NVS | Dados de negócios (Farm/Lot/Animal) persistidos na NVS com busca em array JSON dinâmico sob demanda para economizar RAM |
| Redundância de Rede | Conexão de dados Wi-Fi STA priorizada sobre celular PPP para economizar custos e consumo. Suspensão celular automática enquanto Wi-Fi possui IP válido. |

## Errors Encountered
| Error | Attempt | Resolution |
|-------|---------|------------|
| Falha build idf.py local | 1 | Comando idf.py indisponível no path do sistema local. Validação lógica feita com simuladores Python. |

## Notes
- Atualizar o status de cada fase conforme avançar no desenvolvimento.
- Manter o checklist de tarefas atualizado para acompanhamento no Git.
