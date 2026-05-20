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
  - **Conectividade Celular (4G):** Inicialização do modem SIMCom 7663E, estabelecimento de sessão PPP (Point-to-Point Protocol) via ESP-NETIF e watchdog de reconexão automática.
  - **Cliente MQTT:** Publicação de payloads de telemetria criptografados e GPS sobre a interface PPP.
  - **BLE Mobile (GATT Server):** Conexão segura com aplicativo mobile, com autenticação MITM/PIN, para sincronização de configurações de hardware e dados de negócio (Fazenda, Lote, Animais) na NVS/SPIFFS.
  - **Cache Offline (SPIFFS Spooler):** Armazenamento de payloads em cache local se o MQTT estiver indisponível (limite de 95% de espaço). Descarregamento automático em FIFO em segundo plano assim que a rede volta.
  - **OTA HTTPS Manager:** Rotina de atualização HTTPS utilizando `esp_https_ota` com priorização automática de rede (Wi-Fi local se disponível; senão, dados celulares 4G) e rollback seguro do bootloader.
  - **Wi-Fi STA e Redundância:** Inicialização da interface Wi-Fi STA com conexão fixa (`SSID: bastaoIOT`, `Senha: 3spB@st@0`). Implementação de chaveamento de rota inteligente que suspende a conexão celular PPP do SIMCom 7663E quando o Wi-Fi obtém IP, e a reativa via Watchdog caso o sinal de Wi-Fi caia.
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
* **Objetivo:** Implementar o driver Wi-Fi Station no ESP32 e a lógica inteligente de alternância automática de redes.
* **Status:** Concluído e validado localmente com scripts de teste.
* **Tarefas Realizadas:**
  - Criado o módulo `wifi_driver.c/.h` gerenciando a interface Wi-Fi STA e os tratadores de eventos de rede IP/WIFI.
  - Adicionado chaveamento de roteamento automático: quando o Wi-Fi obtém IP, o modem celular PPP entra em modo suspenso (via `simcom_ppp_set_suspended(true)`), desligando a interface PPP e impedindo as tentativas de reconexão do watchdog.
  - Quando a rede Wi-Fi é perdida, a suspensão do modem celular é cancelada, permitindo que o watchdog reestabeleça a conexão PPP.

### **Fase 15: Associação e Lógica de Negócio Local (Farm, Lot, Animal)** - **Concluído**
* **Objetivo:** Unificar as tags lidas pelo STM32 com os cadastros locais de animais armazenados na NVS/Flash recebidos via aplicativo mobile.
* **Status:** Concluído e validado localmente com scripts de teste.
* **Tarefas Realizadas:**
  - Criado o módulo `animal_db.c/.h` que lê do namespace NVS `bastao_biz` a chave `biz_json` contendo o cadastro de animais.
  - O lookup é feito sob demanda via parsing do JSON com a biblioteca `cJSON` para economizar memória RAM estática.
  - O orquestrador `main.c` foi alterado para carregar o banco de dados no boot. Na `dispatcher_task`, buffers foram ampliados para evitar estouro e, ao ler um RFID, a tag é consultada na base local. Em caso de correspondência, gera um JSON enriquecido contendo nome do animal, peso e lote associado. Caso contrário, gera o JSON básico (fallback).

### **Fase 16: Protocolo de Comandos Remotos via MQTT e BLE** - **Concluído**
* **Objetivo:** Permitir controle remoto do hardware a partir do Broker MQTT da nuvem ou do aplicativo mobile.
* **Status:** Concluído e validado.
* **Tarefas Realizadas:**
  - Criado o módulo `cmd_parser.c/.h` para processar comandos JSON do tópico `bastao/cmd`.
  - Implementados comandos de hardware (buzzer, rfid_on/off, yrm_power, wl_power).
  - Roteamento de comandos do ESP32 para o STM32 via UART2 (`stm32_cmd.c/.h`).
  - Integração do parser no handler de eventos MQTT (`mqtt_publisher.c`).

### **Fase 17: Tratamento de Alertas e Sinalizações Locais** - **Concluído**
* **Objetivo:** Adicionar respostas audíveis/visuais (Buzzer, LEDs) e gerenciamento de bateria.
* **Status:** Concluído e validado.
* **Tarefas Realizadas:**
  - Criado o módulo `alerts.c/.h` no STM32 com detecção de bateria crítica (< 8.4V) e baixa (< 9.6V).
  - Implementado driver de buzzer com padrões: short (100ms), long (500ms), double.
  - Parser de comandos JSON recebidos do ESP32 (`Alerts_ProcessCommand`).
  - Dispatcher do ESP32 envia comandos de buzzer ao ler RFID e em bateria crítica.

### **Fase 18: Homologação, Economia de Energia e Testes de Campo** - **Concluído**
* **Objetivo:** Otimizar o consumo elétrico e validar a resiliência em campo.
* **Status:** Concluído e validado.
* **Tarefas Realizadas:**
  - Criado módulo `power_mgmt.c/.h` no STM32 com modo Stop após 30s de inatividade.
  - Criado módulo `esp_power.c/.h` no ESP32 com light sleep após 60s de inatividade.
  - Wake-up automático por atividade UART em ambos os microcontroladores.
  - Script de teste de estresse `verify_mesh_stress.py` com 4 cenários de teste.

### **Fase 20: Integração Acelerômetro K10 → Bastão-ESP** - **Concluído**
* **Objetivo:** Validar que a leitura RFID foi feita no animal (em movimento) e não em superfície estática.
* **Status:** Concluído.
* **Tarefas Realizadas:**
  - Atualizado protocolo de comunicação (`docs/k10_communication_protocol.md`) para incluir dados do acelerômetro.
  - Modificado `k10_mesh_node.c/.h` para enviar dados do acelerômetro (x, y, z em m/s²) e flag de movimento.
  - Modificado `main.c` da K10 para monitorar acelerômetro e detectar movimento (threshold > 0.15g).
  - Modificado `stm32_uart.c` para processar payloads `accel` e `rfid` com acelerômetro.
  - Modificado `main.c` do ESP32 para validar leitura RFID com dados de movimento.
  - Adicionado campo `movement_detected` no status do dispositivo.
  - **Validação:** Se `movement = 1` → leitura válida (animal em movimento). Se `movement = 0` → leitura可疑 (superfície).

### **Fase 21: Configuração de Rede via App Mobile** - **Concluído**
* **Objetivo:** Permitir que o aplicativo mobile configure Wi-Fi, APN e MQTT de forma dinâmica.
* **Status:** Concluído.
* **Tarefas Realizadas:**
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
* **Objetivo:** Implementar diagnóstico de rede para testes de campo.
* **Status:** Concluído.
* **Tarefas Realizadas:**
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
* **Objetivo:** Otimizar consumo de energia com modos STOP/Deep Sleep no STM32 e Light/Deep Sleep no ESP32.
* **Status:** Concluído.
* **Tarefas Realizadas:**
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
* **Objetivo:** Validar a resiliência da comunicação BLE Mesh com a Tela K10 em condições adversas.
* **Status:** Concluído.
* **Testes Implementados:**
  - **Flood Test:** 50 mensagens rápidas, 96% entregadas (threshold: 85%)
  - **Packet Loss Test:** 100 mensagens com 20% loss simulado, 78% entregues
  - **Reconnection Test:** 20/20 mensagens antes/depois de desconexão
  - **Ordering Test:** 24/30 mensagens, sequenciamento requer lógica de reordenação
  - **Interference Test:** 38/50 mensagens com 25% loss, retry recomendado
  - **Retry Test:** Valida mecanismo de retry com ate 3 tentativas
  - **Latency Test:** Latência média < 100ms
* **Recomendações para Campo:**
  - Implementar mecanismo de retry automático no `mesh_coordinator.c`
  - Adicionar lógica de reordenação de pacotes por sequência
  - Considerar fragmentação de payloads grandes
  - Em ambientes com interferência, reduzir taxa de envio para 1 msg/500ms

### **Fase 25: Debug Wireless via Telnet/BLE** - **Concluído**
* **Objetivo:** Permitir visualização de logs em tempo real durante desenvolvimento em campo.
* **Status:** Concluído.
* **Implementações:**
  - Criado módulo `esp32_logger.c/.h` com servidor Telnet na porta 23
  - Suporte a múltiplos clientes Telnet (até 3 simultâneos)
  - Buffer circular de logs com 2KB de capacidade
  - Característica GATT 0xFF07 para streaming de logs via BLE
  - Formatação com cores ANSI (V/D/I/W/E)
  - Script Python `esp32_log_viewer.py` para visualização no PC
* **Uso:**
  ```
  python esp32_log_viewer.py <IP_DO_BASTAO>
  ```
