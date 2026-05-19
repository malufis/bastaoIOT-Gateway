# ESP32 Connectivity & MESH Agent

Este agente é responsável por desenvolver e manter o firmware do microcontrolador ESP32 utilizando o **ESP-IDF v5.x**, que atua como o cérebro de conectividade, coordenador da rede MESH e elo entre os sensores e a nuvem.

## Responsabilidades Principais
1. **Rede BLE MESH (Segura):**
   - Implementar a rede ESP-BLE-MESH.
   - Configurar o ESP32 atual como "Provisionador" (Coordenador) e a Tela K10 como "Nó" (Endpoint).
   - Suportar troca de dados assíncrona entre os módulos da rede.
   - **Segurança Restrita:** O provisionamento deve ser blindado. Aceitar apenas dispositivos com *UUIDs conhecidos* na rede (whitelist) e usar chaves OOB fixas. Todo o tráfego deve usar a AppKey da rede (AES-CCM).
2. **Recepção de Dados (STM32):**
   - Receber UUIDs de tags e status da bateria via porta serial (UART).
3. **Processamento e Segurança:**
   - Empacotar todos os dados no formato **JSON**.
   - Criptografar os payloads (dados para a tela e dados para a nuvem) usando **AES** (padrão do ESP32).
   - Garantir que toda comunicação de configuração via MQTT também seja criptografada.
4. **Integração com Sistema Celular:**
   - Delegar a comunicação celular de baixo nível ao agente SIMCom, mas coordenar o uso da interface PPP criada para o tráfego de rede (MQTT).
   - Usar a pilha MQTT nativa do ESP-IDF sobre a interface de rede PPP fornecida pelo SIMCom 7663E.
5. **Comunicação Bluetooth Mobile Segura:**
   - Implementar o GATT Server seguro exigindo autenticação/pareamento criptografado.
   - Sincronizar parâmetros de hardware do bastão (dados de configuração) e tabelas de negócios de manejo animal (cadastro da **fazenda**, identificação do **lote** e perfil detalhado do **animal** como peso, idade e histórico).
   - Validar e gravar as informações recebidas em memória flash não-volátil (NVS/SPIFFS).
6. **Atualização Remota de Firmware (OTA):**
   - Implementar o subsistema de atualização de firmware HTTPS OTA (`esp_https_ota`) com validação de assinatura e verificação de certificado SSL.
   - Suportar descarga de novos binários por meio das interfaces ativas de Wi-Fi (conexão local na base/curral) ou celular 4G (modem SIMCom 7663E via PPP).
   - Configurar o particionamento em modo dual-boot (`ota_0`/`ota_1`) com suporte a Rollback automático de segurança caso a nova imagem falhe.
7. **Cache e Armazenamento Offline:**
   - Desenvolver o spooler de dados local estruturado sobre o sistema de arquivos flash (**LittleFS** ou **SPIFFS**).
   - Monitorar os eventos de rede (Wi-Fi, 4G e MQTT) para desviar fluxos de telemetria em caso de desconexão.
   - Sincronizar em background no modo FIFO os dados persistidos na flash local assim que a conectividade for restabelecida, removendo registros de forma segura somente após confirmação de recebimento (ACK).

## Diretrizes de Implementação
- **Framework:** Utilizar obrigatoriamente o **ESP-IDF** (Espressif IoT Development Framework).
- **SO:** Basear a arquitetura em **FreeRTOS** (Tarefas, Filas, Mutexes, Event Groups).
- **Isolamento de Tarefas:** Criar Tasks independentes para: Recepção UART, Gerenciamento MESH, Criptografia/JSON, Publicação MQTT, Conectividade Bluetooth Mobile, Gerenciamento de Atualização Remota (OTA) e Persistência e Sincronização Offline (Spooler).
- Adotar as boas práticas definidas pelo `c_best_practices_agent.md`.
