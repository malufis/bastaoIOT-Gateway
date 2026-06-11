---
name: simcom-7663e
description: Diretrizes para integracao do modem SIMCom 7663E, comandos AT, estabelecimento de conexao PPP e obtencao de coordenadas GPS no ESP32.
---

# SIMCom 7663E Cellular & PPP Skill

Esta skill rege a integracao do modulo SIMCom 7663E ao ecossistema do ESP32, fornecendo conectividade de internet celular e servicos de localizacao.

## Responsabilidades Principais
1. **Comunicacao em Nivel de Hardware:**
   - Interface com ESP32 via porta Serial (Pinos 17 e 18).
2. **Inicializacao e Configuracao via Comandos AT:**
   - Enviar comandos AT para inicializacao do modulo.
   - Configurar APN, credenciais de operadora e registrar na rede 4G.
   - Tratar respostas e erros dos comandos AT.
3. **Estabelecimento de Interface PPP (Point-to-Point Protocol):**
   - Mudar o modulo 7663E do modo de comando para o modo de dados PPP.
   - Integrar a interface PPP com a camada de rede LwIP do ESP-IDF (para que o ESP32 possa fazer sockets nativos/MQTT por cima do celular).
4. **Localizacao:**
   - Obter dados de posicao via GPS/GLONASS utilizando os comandos AT especificos do 7663E e repassa-los ao fluxo principal de dados.

## Diretrizes de Implementacao
- **Framework:** Integrar com o componente `esp_modem` (ou DTE/DCE abstraction customizada) do ESP-IDF.
- **Robustez:** Implementar rotinas de reconexao automatica em caso de queda de sinal celular ou falha no estabelecimento da sessao PPP.
- Isolar as strings de comandos AT e logica de parsing do restante do firmware ESP32 para facilitar manutenibilidade.
- Adotar as boas praticas de programacao C.
