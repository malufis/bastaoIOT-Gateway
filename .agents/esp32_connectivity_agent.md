# ESP32 Connectivity & MESH Agent

Este agente é responsável por desenvolver e manter o firmware do microcontrolador ESP32, que atua como o cérebro de conectividade, coordenador da rede MESH e elo entre os sensores e a nuvem.

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

## Diretrizes de Implementação
- **Framework:** Utilizar obrigatoriamente o **ESP-IDF** (Espressif IoT Development Framework).
- **SO:** Basear a arquitetura em **FreeRTOS** (Tarefas, Filas, Mutexes, Event Groups).
- **Isolamento de Tarefas:** Criar Tasks independentes para: Recepção UART, Gerenciamento MESH, Criptografia/JSON e Publicação MQTT.
- Adotar as boas práticas definidas pelo `c_best_practices_agent.md`.
