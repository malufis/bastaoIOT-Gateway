---
name: esp32-connectivity
description: Diretrizes de conectividade do ESP32 utilizando ESP-IDF v5.5.2, incluindo FreeRTOS, BLE MESH, UART (com STM32), MQTT seguro, GATT Server e atualizacoes OTA.
---

# ESP32 Connectivity & MESH Skill

Esta skill rege o desenvolvimento e manutencao do firmware do microcontrolador ESP32 utilizando o **ESP-IDF v5.5.2**, que atua como o cerebro de conectividade, coordenador da rede MESH e elo entre os sensores e a nuvem.

## Responsabilidades Principais
1. **Rede BLE MESH (Segura):**
   - Implementar a rede ESP-BLE-MESH.
   - Configurar o ESP32 atual como "Provisionador" (Coordenador) e a Tela K10 como "No" (Endpoint).
   - Suportar troca de dados assincrona entre os modulos da rede.
   - **Seguranca Restrita:** O provisionamento deve ser blindado. Aceitar apenas dispositivos com *UUIDs conhecidos* na rede (whitelist) e usar chaves OOB fixas. Todo o trafego deve usar a AppKey da rede (AES-CCM).
2. **Recepcao de Dados (STM32):**
   - Receber UUIDs de tags e status da bateria via porta serial (UART1, pinos IO13-RX / IO14-TX) conectada ao STM32 (PA2-TX / PA3-RX). Referencia: [esquematico_placa.pdf](file:///d:/git/Bastao/Bastao-ESP/Manual/esquematico_placa.pdf).
3. **Processamento e Seguranca:**
   - Empacotar todos os dados no formato **JSON**.
   - Criptografar os payloads (dados para a tela e dados para a nuvem) usando **AES** (padrao do ESP32).
   - Garantir que toda comunicacao de configuracao via MQTT tambem seja criptografada.
4. **Integracao com Sistema Celular:**
   - Delegar a comunicacao celular de baixo nivel ao agente SIMCom, mas coordenar o uso da interface PPP criada para o trafego de rede (MQTT).
   - Usar a pilha MQTT nativa do ESP-IDF sobre a interface de rede PPP fornecida pelo SIMCom 7663E.
5. **Comunicacao Bluetooth Mobile Segura:**
   - Implementar o GATT Server seguro exigindo autenticacao/pareamento criptografado.
   - Sincronizar parametros de hardware do bastao (dados de configuracao) e tabelas de negocios de manejo animal (cadastro da **fazenda**, identificacao do **lote** e perfil detalhado do **animal** como peso, idade e historico).
   - Validar e gravar as informacoes recebidas em memoria flash nao-volatil (NVS/SPIFFS).
6. **Atualizacao Remota de Firmware (OTA):**
   - Implementar o subsistema de atualizacao de firmware HTTPS OTA (`esp_https_ota`) com validacao de assinatura e verificacao de certificado SSL.
   - Suportar descarga de novos binarios por meio das interfaces ativas de Wi-Fi (conexao local na base/curral) ou celular 4G (modem SIMCom 7663E via PPP).
   - Configurar o particionamento em modo dual-boot (`ota_0`/`ota_1`) com suporte a Rollback automatico de seguranca caso a nova imagem falhe.
7. **Cache e Armazenamento Offline:**
   - Desenvolver o spooler de dados local estruturado sobre o sistema de arquivos flash (**LittleFS** ou **SPIFFS**).
   - Monitorar os eventos de rede (Wi-Fi, 4G e MQTT) para desviar fluxos de telemetria em caso de desconexao.
   - Sincronizar em background no modo FIFO os dados persistidos na flash local assim que a conectividade for restabelecida, removendo registros de forma segura somente apos confirmacao de recebimento (ACK).

## Diretrizes de Implementacao
- **Framework:** Utilizar obrigatoriamente o **ESP-IDF v5.5.2** (Espressif IoT Development Framework).
- **SO:** Basear a arquitetura em **FreeRTOS** (Tarefas, Filas, Mutexes, Event Groups).
- **Isolamento de Tarefas:** Criar Tasks independentes para: Recepcao UART, Gerenciamento MESH, Criptografia/JSON, Publicacao MQTT, Conectividade Bluetooth Mobile, Gerenciamento de Atualizacao Remota (OTA) e Persistencia e Sincronizacao Offline (Spooler).
- **Arquitetura Modular:** E obrigatoria a separacao de responsabilidades em bibliotecas de suporte (par `.h/.c`), mantendo o `main.c` focado estritamente em orquestrar as inicializacoes e tasks.
- **Padrao de Documentacao (Doxygen):** Descrever toda funcao detalhadamente contendo `@brief`, `@param[in/out]`, `@return`, `@pre`, `@post` e possiveis efeitos colaterais (`@warning`/`@note`).
- Adotar as boas praticas de programacao C.
