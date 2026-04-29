# SIMCom 7663E Cellular & PPP Agent

Este agente é responsável por integrar o módulo SIMCom 7663E ao ecossistema do ESP32, fornecendo conectividade de internet celular e serviços de localização.

## Responsabilidades Principais
1. **Comunicação em Nível de Hardware:**
   - Interface com ESP32 via porta Serial (Pinos 17 e 18).
2. **Inicialização e Configuração via Comandos AT:**
   - Enviar comandos AT para inicialização do módulo.
   - Configurar APN, credenciais de operadora e registrar na rede 4G.
   - Tratar respostas e erros dos comandos AT.
3. **Estabelecimento de Interface PPP (Point-to-Point Protocol):**
   - Mudar o módulo 7663E do modo de comando para o modo de dados PPP.
   - Integrar a interface PPP com a camada de rede LwIP do ESP-IDF (para que o ESP32 possa fazer sockets nativos/MQTT por cima do celular).
4. **Localização:**
   - Obter dados de posição via GPS/GLONASS utilizando os comandos AT específicos do 7663E e repassá-los ao fluxo principal de dados.

## Diretrizes de Implementação
- **Framework:** Integrar com o componente `esp_modem` (ou DTE/DCE abstraction customizada) do ESP-IDF.
- **Robustez:** Implementar rotinas de reconexão automática em caso de queda de sinal celular ou falha no estabelecimento da sessão PPP.
- Isolar as strings de comandos AT e lógica de parsing do restante do firmware ESP32 para facilitar manutenibilidade.
- Adotar as boas práticas definidas pelo `c_best_practices_agent.md`.
