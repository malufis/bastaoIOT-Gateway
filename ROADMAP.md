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

### **Fase 16: Protocolo de Comandos Remotos via MQTT e BLE**
* **Objetivo:** Permitir controle remoto do hardware a partir do Broker MQTT da nuvem ou do aplicativo mobile.
* **Tarefas:**
  - Estender o parser JSON do comando recebido em `bastao/cmd`.
  - Implementar comandos de alteração de configurações de hardware (ex: potência de RF do YRM100, tempo de varredura, ligar/desligar leitor WL-134).
  - Roteamento de comandos de hardware do ESP32 para o STM32 via UART2 (ex: ESP32 envia comando serial e o STM32 ajusta os pinos PWR ou envia comandos seriais adicionais ao YRM100).

### **Fase 17: Tratamento de Alertas e Sinalizações Locais**
* **Objetivo:** Adicionar respostas audíveis/visuais (Buzzer, LEDs) e gerenciamento de bateria.
* **Tarefas:**
  - Definir lógica no STM32 para medir a bateria via ADC e alertar no JSON se estiver abaixo de 15% de tensão.
  - Implementar comandos seriais do ESP32 para o STM32 acionar avisos sonoros no buzzer (ex: bipe curto para leitura de tag com sucesso, bipe longo para erro de cache cheio ou bateria crítica).

### **Fase 18: Homologação, Economia de Energia e Testes de Campo**
* **Objetivo:** Otimizar o consumo elétrico e validar a resiliência em campo.
* **Tarefas:**
  - Implementar modos de baixo consumo (Light Sleep/Deep Sleep) no STM32 e ESP32 quando não houver atividade de sensores por determinado tempo.
  - Testes de estresse de perdas de pacotes na rede Mesh com a Tela K10.
