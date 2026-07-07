# Roadmap do Projeto: Sistema Bastão-ESP

Este documento é o plano de trabalho e guia definitivo do estado atual e próximos passos do projeto. Ele serve como referência rápida para desenvolvedores e agentes de IA não precisarem reler todo o codebase para se localizarem.

---

## 1. Estado Atual do Projeto (Onde Estamos)

O projeto Bastão-ESP é composto por dois microcontroladores operando em conjunto:

### 1.1. Firmware STM32 (Módulo de Sensoriamento) - **Concluído**

- **Arquitetura:** STM32CubeIDE (Bare-Metal + HAL) rodando na MCU STM32G070CBTx.
- **Funcionalidades:**
  - **Leitor WL-134 (LF):** Parsing do protocolo serial a 9600 8N2 via interrupção com buffer circular, conversão LSB-first para decimal e cálculo de checksum XOR.
  - **Leitor YRM100 (UHF):** Emissão de polling a cada 200ms, parsing de pacotes iniciados com `0xBB` e checksum módulo 256.
  - **Medição de Bateria:** Conversão analógica via ADC (divisor de tensão 100k/10k) com envio a cada 5 segundos.
  - **Saída:** Envio de dados formatados em JSON via UART2 a 115200 8N1 para o ESP32.

### 1.2. Firmware ESP32 (Módulo de Conectividade) - **Concluído**

- **Arquitetura:** ESP-IDF v5.x integrado com FreeRTOS.
- **Funcionalidades:**
  - **Task UART Receiver:** Escuta de JSONs vindos do STM32 de forma assíncrona.
  - **Módulo de Segurança:** Criptografia simétrica AES-256-CBC com PKCS#7 padding via mbedTLS.
  - **BLE Mesh Coordinator:** Inicialização do stack Mesh no modo Coordenador para comunicação local segura com a Tela K10.
  - **Conectividade Celular (4G):** Inicialização do modem SIMCom 7663E, estabelecimento de conexão direta via comandos AT utilizando o motor MQTT integrado do próprio modem (sem PPP), com suporte a Dual SIM (Slot 0 e Slot 1) com CCID e watchdog de reconexão automática.
  - **Cliente MQTT:** Publicação de payloads de telemetria criptografados e GPS sobre a interface ativa (Wi-Fi local via esp_mqtt nativo, ou Celular 4G via comandos AT-MQTT integrados no modem SIMCom).
  - **BLE Mobile (GATT Server):** Conexão segura com aplicativo mobile, com autenticação MITM/PIN, para sincronização de configurações de hardware e dados de negócio (Fazenda, Lote, Animais) na NVS/SPIFFS, exibindo o status detalhado do SIM (presença, slot ativo e CCID).
  - **Cache Offline (SPIFFS Spooler):** Armazenamento de payloads em cache local se o MQTT estiver indisponível (limite de 95% de espaço). Descarregamento automático em FIFO em segundo plano assim que a rede volta.
  - **OTA HTTPS Manager:** Rotina de atualização HTTPS utilizando `esp_https_ota` com suporte de rede (Wi-Fi local apenas, devido à remoção de roteamento celular TCP/IP nativo) e rollback seguro do bootloader.
  - **Wi-Fi STA e Redundância:** Inicialização da interface Wi-Fi STA com conexão fixa (`SSID: bastaoIOT`, `Senha: 3spB@st@0`). Implementação de gerenciador de conectividade centralizado (`manage_connectivity`) que orquestra os modos de rede (Wi-Fi Only, Cellular Only, Wi-Fi & Cellular/Auto) e suaviza as reconexões de Wi-Fi para evitar desconexões da sessão celular 4G MQTT ativa.
  - **Banco de Dados de Animais (Local):** Módulo local `animal_db` que carrega a base JSON de negócios da NVS (`biz_json`, namespace `bastao_biz`) e realiza busca linear sob demanda por tag RFID. Enriquece as mensagens do despachante adicionando nome, peso e lote aos JSONs transmitidos se a tag for encontrada.

### 1.3. Pipeline de Testes e Validação - **Concluído**

- Scripts de validação em Python (`teste_automatizado/`):
  - `validate_rfid.py`: Validação de decodificação hexadecimal LF/UHF.
  - `verify_encryption.py`: Validação de compatibilidade de criptografia AES.
  - `verify_ble_gatt.py`: Validação de características e pareamento Bluetooth.
  - `verify_mqtt_payload.py`: Teste de decifração de dados no broker MQTT.
  - `verify_offline_cache.py`: Validação da ordenação FIFO do cache SPIFFS.
  - `verify_ota.py`: Validação do recebimento e parsing de comando OTA via MQTT.
  - `verify_redundancy.py`: Validação do chaveamento automático de rede e suspensão celular sob Wi-Fi ativo.
  - `verify_business_enrichment.py`: Validação do banco de dados de animais local e enriquecimento de telemetrias.

---

## 2. Roadmap e Próximos Passos (Para Onde Vamos)

As próximas etapas cobrem a implementação do Wi-Fi STA, a inteligência de comutação de rede, a lógica de negócio local com tags, e comandos avançados na nuvem.

### **Fase 14: Integração de Wi-Fi e Controle de Redundância de Rede** - **Concluído**

- **Objetivo:** Implementar o driver Wi-Fi Station no ESP32 e a lógica inteligente de alternância automática de redes.

- **Status:** Concluído e validado localmente com scripts de teste.
- **Tarefas Realizadas:**
  - Criado o módulo `wifi_driver.c/.h` gerenciando a interface Wi-Fi STA e os tratadores de eventos de rede IP/WIFI.
  - Adicionado chaveamento de roteamento automático: quando o Wi-Fi obtém IP, o modem celular entra em modo suspenso (via `simcom_driver_set_suspended(true)`), desligando a conexão MQTT celular e impedindo as tentativas de reconexão do watchdog.
  - Quando a rede Wi-Fi é perdida, a suspensão do modem celular é cancelada, permitindo que o watchdog reestabeleça a conexão.

### **Fase 15: Associação e Lógica de Negócio Local (Farm, Lot, Animal)** - **Concluído**

- **Objetivo:** Unificar as tags lidas pelo STM32 com os cadastros locais de animais armazenados na NVS/Flash recebidos via aplicativo mobile.

- **Status:** Concluído e validado localmente com scripts de teste.
- **Tarefas Realizadas:**
  - Criado o módulo `animal_db.c/.h` que lê do namespace NVS `bastao_biz` a chave `biz_json` contendo o cadastro de animais.
  - O lookup é feito sob demanda via parsing do JSON com a biblioteca `cJSON` para economizar memória RAM estática.
  - O orquestrador `main.c` foi alterado para carregar o banco de dados no boot. Na `dispatcher_task`, buffers foram ampliados para evitar estouro e, ao ler um RFID, a tag é consultada na base local. Em caso de correspondência, gera um JSON enriquecido contendo nome do animal, peso e lote associado. Caso contrário, gera o JSON básico (fallback).

### **Fase 16: Protocolo de Comandos Remotos via MQTT e BLE** - **Concluído**

- **Objetivo:** Permitir controle remoto do hardware a partir do Broker MQTT da nuvem ou do aplicativo mobile.

- **Status:** Concluído e validado.
- **Tarefas Realizadas:**
  - Criado o módulo `cmd_parser.c/.h` para processar comandos JSON do tópico `bastao/cmd`.
  - Implementados comandos de hardware (buzzer, rfid_on/off, yrm_power, wl_power).
  - Roteamento de comandos do ESP32 para o STM32 via UART2 (`stm32_cmd.c/.h`).
  - Integração do parser no handler de eventos MQTT (`mqtt_publisher.c`).

### **Fase 17: Tratamento de Alertas e Sinalizações Locais** - **Concluído**

- **Objetivo:** Adicionar respostas audíveis/visuais (Buzzer, LEDs) e gerenciamento de bateria.

- **Status:** Concluído e validado.
- **Tarefas Realizadas:**
  - Criado o módulo `alerts.c/.h` no STM32 com detecção de bateria crítica (< 8.4V) e baixa (< 9.6V).
  - Implementado driver de buzzer com padrões: short (100ms), long (500ms), double.
  - Parser de comandos JSON recebidos do ESP32 (`Alerts_ProcessCommand`).
  - Dispatcher do ESP32 envia comandos de buzzer ao ler RFID e em bateria crítica.

### **Fase 18: Homologação, Economia de Energia e Testes de Campo** - **Concluído**

- **Objetivo:** Otimizar o consumo elétrico e validar a resiliência em campo.

- **Status:** Concluído e validado.
- **Tarefas Realizadas:**
  - Criado módulo `power_mgmt.c/.h` no STM32 com modo Stop após 30s de inatividade.
  - Criado módulo `esp_power.c/.h` no ESP32 com light sleep após 60s de inatividade.
  - Wake-up automático por atividade UART em ambos os microcontroladores.
  - Script de teste de estresse `verify_mesh_stress.py` com 4 cenários de teste.

### **Fase 20: Integração Acelerômetro K10 → Bastão-ESP** - **Concluído**

- **Objetivo:** Validar que a leitura RFID foi feita no animal (em movimento) e não em superfície estática.

- **Status:** Concluído.
- **Tarefas Realizadas:**
  - Atualizado protocolo de comunicação (`docs/k10_communication_protocol.md`) para incluir dados do acelerômetro.
  - Modificado `k10_mesh_node.c/.h` para enviar dados do acelerômetro (x, y, z em m/s²) e flag de movimento.
  - Modificado `main.c` da K10 para monitorar acelerômetro e detectar movimento (threshold > 0.15g).
  - Modificado `stm32_uart.c` para processar payloads `accel` e `rfid` com acelerômetro.
  - Modificado `main.c` do ESP32 para validar leitura RFID com dados de movimento.
  - Adicionado campo `movement_detected` no status do dispositivo.
  - **Validação:** Se `movement = 1` → leitura válida (animal em movimento). Se `movement = 0` → leitura可疑 (superfície).

### **Fase 21: Configuração de Rede via App Mobile** - **Concluído**

- **Objetivo:** Permitir que o aplicativo mobile configure Wi-Fi, APN e MQTT de forma dinâmica.

- **Status:** Concluído.
- **Tarefas Realizadas:**
  - Expandido `ble_mobile.h` com chaves NVS para Wi-Fi, APN e MQTT.
  - Criadas estruturas `wifi_config_t`, `cellular_config_t`, `mqtt_config_t` e `network_config_t`.
  - Implementadas funções:
    - `ble_mobile_load_network_config()` - Carrega config do NVS
    - `ble_mobile_save_network_config()` - Salva config no NVS
    - `ble_mobile_process_network_json()` - Processa JSON do app
    - `ble_mobile_apply_network_config()` - Aplica nova config
  - Modificado `main.c` para carregar config do NVS no boot
  - Criado script de teste `verify_network_config.py`

### **Fase 22: Monitoramento de Rede Celular** - **Concluído**

- **Objetivo:** Implementar diagnóstico de rede para testes de campo.

- **Status:** Concluído.
- **Tarefas Realizadas:**
  - Criada enum `cellular_tech_t` (NONE, 2G, 3G, 4G_LTE, NB_IOT, UNKNOWN).
  - Criada estrutura `cellular_status_t` com RSSI, BER, tecnologia, MCC/MNC, operador, registro e contadores.
  - Implementada função `simcom_ppp_get_signal_quality()` - Leitura AT+CSQ.
  - Implementada função `simcom_ppp_get_network_info()` - Leitura AT+QNWINFO e AT+COPS.
  - Implementada função `simcom_ppp_get_status()` - Consolida status completo.
  - Implementada função `simcom_ppp_diagnostic_json()` - Gera JSON de diagnóstico.
  - Adicionada característica GATT 0xFF06 (Cellular Status) no ble_mobile.c.
  - Implementada função `ble_mobile_notify_cellular_status()` - Notifica app.
  - Implementada função `format_cellular_status_json()` - Formata JSON para app.

### **Fase 23: Modos de Baixo Consumo (Sleep Modes)** - **Concluído**

- **Objetivo:** Otimizar consumo de energia com modos STOP/Deep Sleep no STM32 e Light/Deep Sleep no ESP32.

- **Status:** Concluído.
- **Tarefas Realizadas:**
  - **STM32 (power_mgmt.c/h):**
    - Criada enum `power_mode_t` (ACTIVE, SLEEP_STOP, DEEP_SLEEP).
    - Implementada função `Power_EnterDeepSleep()` com wake-up por LSI RTC.
    - Implementada função `Power_GetMode()` para diagnóstico.
    - Implementadas funções auxiliares (wakeup interval, peripheral re-init).
  - **ESP32 (esp_power.c/h):**
    - Criada enum `esp_power_mode_t` (ACTIVE, LIGHT_SLEEP, DEEP_SLEEP).
    - Implementadas funções `esp_power_enter_light_sleep()` e `esp_power_enter_deep_sleep()`.
    - Configuradas fontes de wake-up (timer, GPIO, UART).
    - Desabilitados Wi-Fi/BT no deep sleep para economia máxima.
    - Implementadas funções de diagnostico e configuracao de timeout.
  - **Integração:**
    - Adicionado `esp_power_update()` no loop principal do main.c.
    - Adicionado `esp_power_trigger_wake()` ao receber RFID tag.
  - **Testes:**
    - Criado script `verify_sleep_modes.py` com 5 cenários de teste.
    - Validados modos de consumo e autonomia de bateria.

### **Fase 24: Testes de Estresse em Campo - Rede Mesh K10** - **Concluído**

- **Objetivo:** Validar a resiliência da comunicação BLE Mesh com a Tela K10 em condições adversas.

- **Status:** Concluído.
- **Testes Implementados:**
  - **Flood Test:** 50 mensagens rápidas, 96% entregadas (threshold: 85%)
  - **Packet Loss Test:** 100 mensagens com 20% loss simulado, 78% entregues
  - **Reconnection Test:** 20/20 mensagens antes/depois de desconexão
  - **Ordering Test:** 24/30 mensagens, sequenciamento requer lógica de reordenação
  - **Interference Test:** 38/50 mensagens com 25% loss, retry recomendado
  - **Retry Test:** Valida mecanismo de retry com ate 3 tentativas
  - **Latency Test:** Latência média < 100ms
- **Recomendações para Campo:**
  - Implementar mecanismo de retry automático no `mesh_coordinator.c`
  - Adicionar lógica de reordenação de pacotes por sequência
  - Considerar fragmentação de payloads grandes
  - Em ambientes com interferência, reduzir taxa de envio para 1 msg/500ms

### **Fase 25: Debug Wireless via Telnet/BLE** - **Concluído**

- **Objetivo:** Permitir visualização de logs em tempo real durante desenvolvimento em campo.

- **Status:** Concluído.
- **Implementações:**
  - Criado módulo `esp32_logger.c/.h` com servidor Telnet na porta 23
  - Suporte a múltiplos clientes Telnet (até 3 simultâneos)
  - Buffer circular de logs com 2KB de capacidade
  - Característica GATT 0xFF07 para streaming de logs via BLE
  - Formatação com cores ANSI (V/D/I/W/E)
  - Script Python `esp32_log_viewer.py` para visualização no PC
- **Uso:**

  ```
  python esp32_log_viewer.py <IP_DO_BASTAO>
  ```

### **Fase 26: SMS, Logs de Antena e Subscrições de Configuração** - **Concluído**

- **Objetivo:** Implementar controle e monitoramento avançados pelo modem celular SIMCom 7663E e recepção de parâmetros remotos.

- **Status:** Concluído.
- **Implementações:**
  - **Subscrição de Configuração Remota:** Implementada subscrição dinâmica no tópico MQTT `id/<ID>/config` (derivado do IMEI do bastão) no evento de conexão. Payloads JSON recebidos são processados via `ble_mobile_process_config_json` e persistidos na NVS.
  - **Logs de Erro da Antena/Torre Celular:** Integradas as consultas ativas aos comandos `AT+CPSI?` e `AT+CEER` para ler métricas LTE (RSRP, RSRQ, SINR) e logs de erro de rede. Os dados são atualizados ativamente offline e cacheados quando o PPP está ativo (evitando tráfego AT concorrente) e expostos via característica GATT 0xFF06 / status JSON.
  - **Módulo SMS de Contingência:** Implementada leitura de mensagens não lidas via `AT+CMGL="REC UNREAD"` na task watchdog quando a sessão PPP está inativa. Suporta comandos `BUZZER`, `RFID [ON/OFF]`, `STATUS` e `RESTART`, respondendo ao remetente com informações de telemetria (bateria, rede, slot SIM ativo, CCID e coordenadas de GPS com indicador de fix).

### **Fase 27: Orquestrador de Conectividade Inteligente e Redundância Dual SIM** - **Concluído**

- **Objetivo:** Ajustar a conectividade do Bastão-ESP para suportar múltiplos modos de rede, realizar detecção inicial e extração de CCID em slots Dual SIM, e resolver instabilidades causadas por tentativas de varredura Wi-Fi incessantes sobre o canal de RF do celular.

- **Status:** Concluído.
- **Tarefas Realizadas:**
  - **Detecção de SIM e CCID:** Implementada varredura automática de inicialização que sonda o Slot 0 e falls back para o Slot 1 se necessário, extraindo o CCID (via `AT+CCID`) e validando o chip (via `AT+CPIN?`). Os atributos `sim_present`, `active_sim_slot` e `sim_ccid` são atualizados e expostos no status GATT/JSON.
  - **Suavização do Driver Wi-Fi:** Removido o loop imediato de reconexão do event handler de Wi-Fi STA. Criada a função de desacoplamento `wifi_driver_disconnect()` para evitar o conflito de declarações de Wi-Fi na aplicação.
  - **Orquestrador de Conectividade:** Criada a máquina de estados centralizada `manage_connectivity()` com suporte aos modos de rede (Wi-Fi Only, Cellular Only, Wi-Fi & Cellular/Auto):
    - `NETWORK_MODE_WIFI_ONLY`: Suspende dados celulares, tenta reconectar Wi-Fi a cada 15 segundos.
    - `NETWORK_MODE_CELLULAR_ONLY`: Mantém celular ativo, garante Wi-Fi desligado.
    - `NETWORK_MODE_WIFI_CELLULAR` / `NETWORK_MODE_AUTO`: Quando Wi-Fi está conectado, suspende o 4G PPP. Quando Wi-Fi cai, reativa o 4G PPP imediatamente e executa varreduras de Wi-Fi suaves em background a cada 60 segundos para evitar queda da rede celular ativa.

### **Fase 28: Integracao MQTT com sistemaBastao** - **Concluido**

- **Objetivo:** Estabelecer o fluxo completo de telemetria do Bastao-ESP ate o banco PostgreSQL do sistemaBastao (https://github.com/malufis/sistemaBastao), validando com payload ficticio.

- **Status:** Concluido.
- **Tarefas Realizadas:**
  - **28.1 — Alinhamento do Topico MQTT:** Topico alterado para `agro/bastao/{mac}/telemetry` (compativel com subscribe do sistemaBastao).
  - **28.2 — IV Aleatorio no Payload:** `secure_payload.c` modificado para gerar IV aleatorio via `esp_fill_random()` e prefixa-lo (16 bytes) ao ciphertext no output hex.
  - **28.3 — Padronizacao do JSON de Telemetria:** dispatcher_task enriquece payload com campos `id_brinco`, `latitude`, `longitude`, `nivel_bateria`, `timestamp_rtc` para o sistemaBastao.
  - **28.4 — Credenciais MQTT por numero_serie:** Usa MAC do ESP32 como username MQTT e chave AES (hex) como password, conforme esperado pelo `mqtt_usuarios` do sistema.
  - **28.5 — Script de Teste:** Criado `teste_automatizado/simular_envio_bastao.py` para validar o fluxo completo sem hardware.
- **Validacao:**
  - Topico MQTT `agro/bastao/TESTE001/telemetry` publicado e recebido por subscriber ([OK])
  - Payload criptografado com IV aleatorio decifrado corretamente ([OK])
  - Compatibilidade `secure_payload.c` (ESP32) ↔ `crypto.py` (sistemaBastao) verificada ([OK])
  - Script `verify_mqtt_payload.py` atualizado e aprovado ([OK])
  - Script `simular_envio_bastao.py` funcional com broker standalone ([OK])
  - **28.6 — Teste Real com ESP32 + 4G:** Conexao PPP estabelecida, MQTT conectado ao broker `209.50.240.55:1883`, payload criptografado enviado ([OK]).
  - **28.7 — Cadastro de Bastao via API:** Fazenda `Fazenda Teste Integracao` e bastao `206EF1D4D574` cadastrados no sistemaBastao ([OK]).
  - **28.8 — Keepalive Ajustado:** MQTT keepalive alterado de 10800s para 60s ([OK]).
  - **28.9 — Loop de Teste RFID:** Injecao periodica de RFID a cada 5 minutos via `test_loop_enabled` em `main.c`.
    - **⚠ TEST CODE — deve ser removido antes da producao.**
    - Documentado com `@warning` no codigo e neste roadmap.
  - **28.10 — Config Centralizada via .env:** `generate_config.py` criado, `private_configs.env` como fonte unica de config.

---

### **Fase 29: Refatoração do Firmware STM32 (Modularização)** - **Concluido**

- **Objetivo:** Separar o codigo monolitico do `main.c` em modulos coesos conforme as skills do projeto, seguindo as boas praticas C (SRP, modularidade, buffers circulares).

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **29.1 — `rfid_parser.c/.h`:** Extraido parsing YRM100 (frame `0xBB`) e WL-134 (frame ASCII 30 bytes) do `main.c` para modulo dedicado.
  - **29.2 — `battery_monitor.c/.h`:** Extraido leitura ADC da bateria do `main.c`, adicionado filtro de media movel (8 amostras conforme skill).
  - **29.3 — `circular_buffer.c/.h`:** Extraido structs de buffer circular do `main.h` para modulo generico reutilizavel com API completa (Init, Write, Read, Available, Peek, Flush).
  - **29.4 — Limpeza de `main.h`:** Removido `BATTERY_CRITICAL_THRESHOLD 15.0f` conflitante (valores corretos mantidos em `alerts.h`: 8.4V critico, 9.6V baixo). Removidas declarações movidas para os novos modulos.
  - **29.5 — `Command_Process()`:** Implementado monitoramento de estado do buzzer e sinalizacao LED para bateria critica (piscacao a cada 500ms).
  - **29.6 — Buffers manuais substituidos:** `RFID_Buffer_t` substituido por `rfid_parser.c/.h`, `CMD_Buffer_t` substituido por `cmd_line_buffer[]` com acesso simplificado.

### **Fase 30: Validacao e Robustez do Pipeline STM32 → ESP32** - **Concluido**

- **Objetivo:** Garantir que todos os dados do STM32 cheguem corretamente ao ESP32 sem perdas, e que o pipeline seja resiliente.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **30.1 — Parser `type:alert` no ESP32:** Adicionado tratamento para `{"type":"alert","code":"batt_critical","volt":...}` no `stm32_uart.c`, com novo tipo `DATA_TYPE_ALERT`.
  - **30.2 — `test_loop_enabled` removido:** Codigo de injecao RFID falsa eliminado do `main.c` do ESP32.
  - **30.3 — Fila aumentada:** `stm32_data_queue` ampliado de 10 para 20 itens.
  - **30.4 — Heartbeat STM32:** Adicionado envio periodico de `{"type":"heartbeat"}\n` a cada 30s do STM32 para o ESP32.
  - **30.5 — Watchdog STM32:** Implementado `stm32_uart_is_stm32_alive()` e `stm32_uart_watchdog_reset()`. Se ESP32 nao receber heartbeat do STM32 por >60s, loga erro e marca `bastao_current_status.stm32_alive = 0`.

### **Fase 31: Testes de Integracao e Validacao Final** - **Concluido**

- **Objetivo:** Validar o pipeline completo com ferramentas de teste automatizadas.

- **Status:** Concluido. **88/88 testes passaram (0 falhas).**

- **Scripts Criados:**
  - **31.1 — `verify_stm32_uart.py`:** 35 testes validando formato dos JSONs do STM32 (RFID YRM100/WL134, bateria, alertas, heartbeat, comandos ESP32, watchdog, fila). Validacao de estrutura, tipos, terminacao `\n` e campos obrigatorios.
  - **31.2 — `verify_uart_stress.py`:** 13 cenarios de estresse: burst de 20/50/100 tags, burst misto YRM100+WL134, burst com interrupcao de bateria. Documenta que fila de 20 slots e suficiente para uso real (1-10 tags/s), mas bursts >20 tags sem consumo simultaneo podem perder dados.
  - **31.3 — `verify_bidirectional.py`:** 22 testes de comandos bidirecionais: 3 padrões de buzzer, 6 comandos de power, alertas STM32, heartbeat como ACK, roteamento de comandos via `stm32_cmd.c`.
  - **31.4 — `verify_sleep_wake_uart.py`:** 18 testes de sleep/wake: timeout 30s STOP, wake por UART, wake por RFID, ciclo completo, light sleep ESP32.

---

### **Fase 32: Reset STM32 via GPIO19 e Watchdog com Grace Period** - **Concluido**

- **Objetivo:** Implementar controle de reset do STM32 via GPIO do ESP32 e watchdog inteligente com tolerancia ao tempo de boot.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **32.1 — Pino de reset (GPIO19):** Adicionado `STM32_RESET_PIN GPIO_NUM_19` em `stm32_uart.h`.
  - **32.2 — `stm32_uart_reset_init()`:** Configura GPIO19 como **push-pull** (inicial **LOW**). O GPIO19 aciona um **transistor NPN** que controla o NRST do STM32 com logica **invertida**: LOW = STM32 roda, HIGH = STM32 em reset.
  - **32.3 — `stm32_uart_reset_stm32()`:** Gera pulso de **HIGH** por 100ms (transistor ON, NRST LOW), depois retorna a LOW (transistor OFF, NRST HIGH). O STM32 e reiniciado.
  - **32.4 — Reset na inicializacao:** ESP32 aplica reset no STM32 apos 500ms de boot para garantir sincronizacao do estado.
  - **32.5 — Watchdog com grace period:** Substituido contador fixo de 3 ciclos por logica que so conta falhas apos o primeiro heartbeat recebido (`stm32_uart_has_ever_been_alive()`). Aumentado limiar para 120 ciclos (~120s) para evitar loop infinito de reset durante boot do STM32 (que leva ~5s para enviar primeiro heartbeat).
  - **32.6 — `stm32_uart_has_ever_been_alive()`:** Nova funcao publica que retorna se ao menos um heartbeat foi recebido desde o boot.
  - **32.7 — Correcao de comentarios:** "GPIO23" corrigido para "GPIO19" em `stm32_uart.h` e `main.c`.
  - **32.8 — Circuito de reset com transistor:** Documentado em `PROJETO_BASTAO.md` o schema do transistor NPN entre GPIO19 e NRST (logica invertida).

### **Fase 33: Filtro de Deduplicacao de Leituras RFID** - **Concluido**

- **Objetivo:** Evitar multiplos envios da mesma tag RFID lida repetidamente em intervalo curto (<1s).

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **33.1 — Criado modulo `rfid_dedup.c/.h`:** Tabela circular com 64 entradas para armazenar ultimas tags lidas.
  - **33.2 — Criterios de dedup:**
    - Mesma tag + mesmo modelo de leitor
    - Intervalo < 10 segundos (`RFID_DEDUP_WINDOW_MS`)
    - GPS proximo (< 50m de raio, `RFID_DEDUP_GPS_RADIUS_M`)
    - Se GPS indisponivel, decisao apenas por tag + tempo
  - **33.3 — Insercao no pipeline:** Chamada `rfid_dedup_is_duplicate()` no `dispatcher_task` antes de processar a leitura RFID. Se duplicata, usa `continue` para pular processamento, criptografia e envio MQTT/Mesh.
  - **33.4 — Integracao:** `rfid_dedup_init()` chamado em `app_main()`, arquivo adicionado ao `CMakeLists.txt`.

### **Fase 34: Wake Sources RFID no STM32 Sleep** - **Concluido**

- **Objetivo:** Garantir que o STM32 acorde do modo STOP quando dados chegarem dos leitores RFID (WL-134 e YRM100), nao apenas via comando UART do ESP32.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **34.1 — `HAL_UARTEx_EnableStopMode(&huart3)`:** Habilitado USART3 (WL-134) como fonte de wake no `Power_Sleep()`.
  - **34.2 — `HAL_UARTEx_EnableStopMode(&huart4)`:** Habilitado USART4 (YRM100) como fonte de wake no `Power_Sleep()`.
  - **34.3 — `DisableStopMode` correspondente:** Adicionado desabilitacao ao sair do STOP mode para ambos os perifericos.
  - **Impacto:** STM32 pode agora ser acordado por dados RFID mesmo em repouso, eliminando janelas de dados perdidos.

---

### **Fase 35: Correcao do Circuito de Reset com Transistor NPN** - **Concluido**

- **Objetivo:** Corrigir a logica de reset do STM32 considerando o transistor NPN entre GPIO19 e NRST (logica invertida).

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **35.1 — Logica invertida:** Descoberto que GPIO19 HIGH → transistor ON → NRST LOW (reset). GPIO19 LOW → transistor OFF → NRST HIGH (roda).
  - **35.2 — Modo Push-Pull:** Alterado de `GPIO_MODE_OUTPUT_OD` para `GPIO_MODE_OUTPUT` (push-pull) para acionar corretamente o transistor.
  - **35.3 — `reset_init`:** Inicializa pino em LOW (transistor OFF) para STM32 rodar desde o boot.
  - **35.4 — `reset_stm32`:** Corrigido pulso: HIGH por 100ms (reset), depois LOW (roda).

### **Fase 36: Filtro de Deduplicacao RFID (rfid_dedup)** - **Concluido**

- **Objetivo:** Evitar multiplos envios da mesma tag lida em intervalo <1s.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **36.1 — Tabela circular:** 64 entradas com tag, modelo, timestamp e GPS.
  - **36.2 — Criterios:** Mesma tag + modelo + janela <10s + GPS <50m (se disponivel).
  - **36.3 — Integracao:** `rfid_dedup_is_duplicate()` chamado no `dispatcher_task` antes de processar RFID.

### **Fase 37: Gerenciamento de Chip SIM (DSSS)** - **Concluido**

- **Objetivo:** Implementar troca segura de chip SIM em modo DSSS com comando proprietario `AT*SELECTSIMSLOT`.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **37.1 — Comando correto:** Substituido `AT+SWITCHSIM` por `AT*SELECTSIMSLOT` (proprietario, com `*`).
  - **37.2 — Sequencia DSSS:** `AT+CFUN=0` (desliga radio) → `AT*SELECTSIMSLOT=<slot>` → `AT+CFUN=1` (religa) → espera 5s → `AT+CPIN?`.
  - **37.3 — `at_init_sequence`:** Sondagem dos 2 slots com sequencia DSSS completa.
  - **37.4 — Watchdog:** Swap de slot automatico se `sim_present == false`.
  - **37.5 — Fallbacks removidos:** `AT+SWITCHSIM` e `AT+DUALSIM` removidos (comandos de outra serie).
  - **37.6 — CCID corrigido:** `AT+CCID` → `AT+CICCID` (comando correto do manual).

### **Fase 38: Sincronizacao de Horario (Torre + SNTP)** - **Concluido**

- **Objetivo:** Garantir que o timestamp dos payloads MQTT tenha data/hora correta.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **38.1 — `simcom_ppp_sync_time_from_tower()`:** Leitura da hora via `AT+CCLK?`, parse do formato `"yy/MM/dd,hh:mm:ss±zz"` e ajuste via `settimeofday()`.
  - **38.2 — SNTP:** Inicializacao do cliente SNTP com servidores `pool.ntp.org`, `a.ntp.br`, `b.ntp.br` para sync via Wi-Fi.
  - **38.3 — Logica no loop principal:** Se `!time_is_synced()` e PPP ativo, tenta torre; SNTP roda em background.
  - **38.4 — Log `[HB]`:** Heartbeat a cada ~10s com horario formatado, status GPS, STM32, PPP e MQTT.
  - **38.5 — Log `[MQTT_ENVIO]`:** Debug do payload antes de criptografar, mostrando tag, GPS e timestamp.

---

### **Fase 39: Correcao Sleep STM32 e Wake via UART** - **Concluido**

- **Objetivo:** Aumentar timeout de sleep para evitar travamentos e garantir wake confiavel via UART.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **39.1 — Timeout:** `POWER_SLEEP_TIMEOUT_MS` alterado de 30s para **120s** (2 min).
  - **39.2 — Log sleep/wake:** STM32 envia JSON `{"type":"power","action":"sleep/wake"}` via UART2.
  - **39.3 — Re-arm RX:** `HAL_UART_Receive_IT()` chamado apos wake para huart2/3/4 (seguranca).
  - **39.4 — Wake preventivo:** ESP32 envia `\n` apos 60s sem heartbeat para acordar STM32.
  - **39.5 — Parser power:** `DATA_TYPE_POWER` adicionado no `stm32_uart.c` (elimina warning).

### **Fase 40: Otimizacao da Leitura UART (Blocos) e Core Pinning** - **Concluido**

- **Objetivo:** Acelerar leitura dos dados do STM32 e isolar CPU por nucleo.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **40.1 — Leitura em blocos:** `uart_read_bytes()` alterado de 1 byte (timeout 20ms) para **1024 bytes** por chamada. Linha RFID de 60 bytes processada em <5ms (antes 1.2s).
  - **40.2 — Core pinning:** Todas as tarefas fixadas por nucleo:
    - **Core 0:** `stm32_uart_rx_task` (prio 7), `dispatcher_task` (prio 6)
    - **Core 1:** `mqtt_pub_task`, `simcom_watchdog`, `ppp_rx_task`, `ota_task`, `cache_sync`, `logger`
  - **40.3 — Buffers:** Alocacao de buffer de chunk separado do buffer de linha no RX task.

### **Fase 41: GPS — Logs de Diagnostico Detalhados** - **Concluido**

- **Objetivo:** Adicionar logs `[GPS]` para depuracao da aquisicao de posicao.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **41.1 — Logs INFO:** Todas as etapas do GPS agora usam `ESP_LOGI` (antes `LOGD`), visiveis em console normal.
  - **41.2 — Cold start:** Log de retorno de `AT+CGPSCOLD` e `AT+CGPS=1`.
  - **41.3 — Polling:** Log a cada tentativa com numero da tentativa/total.
  - **41.4 — Parse:** Log do resultado do parse com `parsed`, `fix_mode`, `lat_raw`, `lon_raw`.
  - **41.5 — Fix:** Log completo com fix_mode, lat, lon, alt, velocidade.

---

### **Fase 42: Transição de PPP para Comandos AT Diretos (SIMCom MQTT)** - **Concluido**

- **Objetivo:** Substituir a pilha PPP e sua integração LwIP no modem celular por conexões diretas via comandos AT, utilizando o motor MQTT interno do SIMCom 7663E.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **42.1 — Abandono do PPP:** Removido o uso de `esp_netif_ppp` e a dependência do stack TCP/IP nativo do ESP32 para rotas celulares.
  - **42.2 — Novo Driver Serial (`simcom_driver.c/.h`):** Desenvolvida tarefa contínua `simcom_uart_rx_task` encarregada de parsear URCs (`+CMQTTDELIVER` para mensagens de subscrição e `+CMQTTCONNLOST`) e capturar respostas síncronas de comandos AT por meio de controle de semáforos.
  - **42.3 — Roteamento Dinâmico de Publicação (`mqtt_publisher.c`):** Atualizada a fila e tarefa de envio para rotear mensagens dinamicamente: usa o cliente nativo `esp_mqtt` quando Wi-Fi está ativo, e o cliente `simcom_driver` quando operando sob rede celular 4G.
  - **42.4 — Restrição de OTA ao Wi-Fi:** Atualizada a rotina de atualização `ota_manager.c` para permitir downloads HTTPS apenas sob cobertura de Wi-Fi, reportando erro se disparada em 4G (devido à ausência de gateway PPP).
   - **42.5 — Validação:** Compilação com compilador ESP-IDF validada com sucesso.

---

### **Fase 43: BLE Mesh — Correção CID e Comunicação Coordenador ↔ K10** - **Concluido**

- **Objetivo:** Estabelecer comunicação funcional entre coordenador ESP32 e display K10 via BLE Mesh.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **43.1 — CID Fix:** Descoberto bug crítico: CID=0xFFFF conflita com CID_NVAL do ESP-IDF v5.4. Mudado para 0x02A5 (Espressif) em `mesh_coordinator.c` e `k10_mesh_node.c`.
  - **43.2 — Inicialização assíncrona:** Refatorado provisioner para chain de eventos: `PROV_ENABLE_COMP_EVT` → `ADD_LOCAL_APP_KEY_COMP_EVT` → `BIND_APP_KEY_TO_MODEL_COMP_EVT`.
  - **43.3 — Flags de estado:** Adicionado `local_model_bound`, movido `k10_provisioned=true` para evento `MODEL_APP_BIND`, retry em falha de APP_KEY_ADD.
  - **43.4 — Persistência NVS na K10:** `CONFIG_BLE_MESH_SETTINGS=y` adicionado para persistir subnet entre reboots.
  - **43.5 — GATT Service Change:** `CONFIG_BT_GATTS_SEND_SERVICE_CHANGE_MANUAL=y` adicionado para eliminar warnings.
  - **43.6 — Envio JSON plain:** Removida criptografia AES-256-CBC do path BLE Mesh. K10 não descriptografa. Enviado JSON simples.
  - **43.7 — Hello world removido:** `mesh_coordinator_send_hello_test()` removido após validação da comunicação.

### **Fase 44: GPS + Celular na Tela K10** - **Concluido**

- **Objetivo:** Enviar coordenadas GPS e status da rede celular do Coordenador para o display K10.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **44.1 — GPS periódico:** Coordenador envia JSON GPS a cada 30s via `mesh_coordinator_send_data()`.
  - **44.2 — Cell periódico:** Coordenador envia JSON com RSSI, conectividade e operadora a cada 60s.
  - **44.3 — Parsing K10:** Adicionados parsers para `"type":"gps"` e `"type":"cell"` em `k10_mesh_process_payload()`.
  - **44.4 — Structs K10:** `k10_gps_data_t` (lat, lon, fix, alt, speed) e `k10_cell_status_t` (rssi, connected, operator_name) em `k10_mesh_node.h`.
  - **44.5 — GUI K10:** `lbl_gps_coords` atualizado com dados reais (verde) ou "GPS: SEM SINAL" (vermelho). `icon_4g` fica verde (conectado) ou vermelho (desconectado).
  - **44.6 — Polling no gui_task:** GPS e Cell lidos a cada 500ms no loop principal da K10.

### **Fase 45: Otimização SIMCom (Captura de Tags) + GPS Hot Start** - **Concluido**

- **Objetivo:** Sanar perdas de leituras de tags por conflitos seriais de comandos AT no modem SIMCom 7663E e configurar GPS para Hot Start.

- **Status:** Concluido.

- **Tarefas Realizadas:**
  - **45.1 — Fila MQTT Assíncrona no CELLULAR_ONLY:** A fila `mqtt_publish_queue` e a task `mqtt_pub_task` agora rodam sempre. O dispatcher enfileira tags sem bloquear a thread principal em chamadas AT seriais.
  - **45.2 — Proteção contra Recriações no Watchdog:** UART driver e task de recepção `simcom_rx` não são mais recriados nas desconexões do watchdog, evitando crashes e deadlocks.
  - **45.3 — Sonda DSSS Acelerada:** Adicionada flag `first_init_done` para pular a varredura física de slots e ir direto para o chip ativo durante reinicializações.
  - **45.4 — Métricas de Rede em Cache:** `simcom_driver_get_status` e orchestrator utilizam dados em cache. Comandos AT pesados (`AT+COPS?` e `AT+CEER`) rodam de forma lenta a cada 5 minutos no watchdog task em background.
   - **45.5 — GPS Hot Start via AP_Flash:** GNSS configurado com `AT+CGNSSPWR=1,1` no power on e `AT+CGNSSPWR=0,1` no power off para salvar efemérides em Flash.

---

### **⚠ Fase 46: Investigação — YRM100 não está lendo tags**

- **Objetivo:** Diagnosticar por que o leitor UHF YRM100 não está lendo tags, mesmo com `YRM100_SetTXPower(26)` implementado.
- **Status:** 🔴 **PENDENTE — Necessita investigação em campo.**
- **Hipóteses:**
  - Necessidade de power cycle físico do módulo (pode estar em estado inconsistente)
  - Parsing do frame de resposta pode estar incorreto (cálculo do EPC offset)
  - Hardware: fiação, antena ou módulo danificado
  - Baud rate incompatível entre STM32 e YRM100
- **Ações necessárias:**
  - Verificar com osciloscópio/lógica analyzer se o YRM100 está respondendo ao comando `0xBB 0x00 0x22 0x00 0x00 0x22 0x7E`
  - Testar com um power cycle manual (desligar e ligar alimentação do YRM100)
  - Verificar parsing do frame de resposta (especialmente EPC offset e checksum)

---

### **Fase 66.1: Buzzer via I2S (Speaker NS4168)** - **Concluido**

- **Objetivo:** Implementar driver de áudio para o speaker built-in da K10 (NS4168) via I2S.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Criado `k10_hal/hal_buzzer.c/.h` com driver I2S usando `driver/i2s_std.h` (API nova ESP-IDF v5.4.4).
  - Pinos: BCLK=GPIO0, LRCK=GPIO38, SDO=GPIO45 (MCLK=NC).
  - Geração de sine wave 44100Hz/16-bit/mono.
  - Tipos de beep: short(100ms), long(300ms), double, alert(2kHz).
  - Chamado no `main.c` da K10 a cada leitura RFID.

### **Fase 66.2: Tela "BRINCO LIDO"** - **Concluido**

- **Objetivo:** Exibir tela de confirmação de leitura com feedback visual.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Tela branca fullscreen com "BRINCO LIDO" + número da tag.
  - Auto-dismiss após 3s via `lv_timer_create()`.
  - Texto "GPS SINCRONIZADO" exibido abaixo de lat/lon quando fix=true.
  - Corrigido `lv_font_montserrat_28` → `lv_font_montserrat_24`.

### **Fase 66.3: SPIFFS + Contador Diário** - **Concluido**

- **Objetivo:** Persistir leituras de tags em armazenamento local para contagem diária.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Novo componente `tag_database/` com `tag_database.c/.h`.
  - Partição SPIFFS (2MB em 0x310000) adicionada ao `partitions.csv`.
  - Arquivos JSON por data (`/spiffs/tags/YYYY-MM-DD.json`).
  - Buffer em RAM com auto-save a cada 10 reads.
  - Flash K10 atualizada 4MB→16MB.

### **Fase 66.4: Aba Histórico (Tab2)** - **Concluido**

- **Objetivo:** Exibir histórico de leituras na tela K10.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Reescrito `create_screen_connectivity()` → `create_screen_history()`.
  - Header com ícone olho + total de leituras + tags únicas.
  - Lista scrollável com últimas 20 tags (tag ID + nome animal).
  - Ícone da aba mudado de WiFi→LIST.

### **Fase 66.5: GPS Gate + Triangulação Celular** - **Concluido**

- **Objetivo:** Publicar MQTT somente quando houver localização válida.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Extraído TAC/CID/EARFCN de `AT+CPSI?` em `query_cpsi_metrics()`.
  - Adicionada `cell_tower_get_location()` via Mozilla Location Service HTTPS POST.
  - Dispatcher: GPS优先 → cell tower fallback → sem localização: tag descartada do MQTT.
  - Atualização de partições: ESP32 OTA 4MB cada, SPIFFS 2MB, flash 16MB.

---

### **Fase 67: Aceleração GPS via A-GPS (AT+CAGPS)** - **Concluido**

- **Objetivo:** Acelerar fix GPS de 30s+ para 2-5s usando dados de assistência do servidor AGNSS via 4G.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Implementado `simcom_driver_download_agps()` — sequência completa: aguarda `+CGNSSPWR:READY!`, configura multi-constelação (GPS+BDS+GLONASS), baixa dados AGNSS (`AT+CAGPS`), reinicia GPS com assistência (`AT+CGPSCOLD`).
  - Handler URC `+CGNSSPWR:READY!` no `process_simcom_line()`.
  - Integração no boot (após `configure_apn`) e na reconexão do watchdog.
  - Fallback para modo 3 (GPS+QZSS) se modem não suportar modo 7.

### **Fase 68: Cell Tower Location — Cache Assíncrono** - **Concluido**

- **Objetivo:** Remover bloqueio de 10s do dispatcher RFID causado pelo HTTP POST ao Mozilla Location Service.
- **Status:** Concluido.
- **Tarefas Realizadas:**
  - Cache de lat/lon da torre celular em `simcom_driver.c`.
  - `simcom_driver_update_cell_tower_cache()` atualiza cache em background no orchestrator (a cada 5min).
  - `simcom_driver_get_cell_tower_location()` retorna cache (~0ms, non-blocking).
  - Invalidação automática quando CID da torre muda (handover/roaming).
  - Tags RFID processadas sem delay por geolocalização.

---

### **Fase 44 (Planejada): OTA via Celular (4G)**

- **Objetivo:** Permitir atualização OTA do ESP32 Coordenador mesmo quando conectado apenas via 4G (modo CELLULAR_ONLY).
- **Abordagem proposta:**
  - Baixar o binário via comandos AT HTTP do modem (`AT+HTTPURL`, `AT+HTTPREAD`) para buffer do ESP32.
  - Gravar na partição OTA inativa via `esp_ota_write()`.
  - Manter rollback seguro (partições A/B já existentes).
- **Dependências:** Implementação de download via AT commands no `simcom_driver.c`.

### **Fase 45 (Planejada): OTA na Tela K10**

- **Objetivo:** Adicionar suporte OTA ao display K10.
- **Abordagem proposta:**
  - Adicionar partição `ota_0` no `partitions.csv` da K10 (reduzir tamanho da `factory`).
  - Receber binário via BLE Mesh (pacotes fragmentados) ou UART do ESP32.
  - Usar `esp_ota_begin()` / `esp_ota_write()` / `esp_ota_end()` para gravação.

### **Fase 46 (Planejada): CI/CD e Report de Progresso OTA**

- **Objetivo:** Automatizar build e deploy de firmware, com feedback de status para a nuvem.
- **Abordagem proposta:**
  - GitHub Actions para build automático a cada tag/push.
  - Upload do `.bin` para servidor HTTPS (GitHub Releases ou S3).
  - Publicação MQTT de status OTA (iniciado, baixando X%, sucesso/erro).

