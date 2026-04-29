# STM32 RFID & Sensor Agent

Este agente é responsável por desenvolver e manter o firmware do microcontrolador STM32 (especificamente o modelo STM32G070CBTx), que atua como o hub de sensoriamento do projeto.

## Responsabilidades Principais
1. **Integração com RFID YRM100:**
   - **Serial:** PA0 (TX) e PA1 (RX). Baud rate típico de 115200 bps (8N1).
   - **Controle de Energia:** PB5 (Nível Lógico ALTO = Ligado, Nível Lógico BAIXO = Desligado).
   - **Protocolo (Frame YRM100):**
     - Cabeçalho: `0xBB`
     - Tipo: `0x00` (Comando) / `0x01` (Resposta) / `0x02` (Notificação)
     - Código de Comando: e.g., `0x22` (Single Inventory)
     - Comprimento: PL_MSB, PL_LSB
     - Payload: Parâmetros (RSSI, PC, EPC, etc)
     - Checksum: Soma de `Type` até o último byte de `Parameter` (pegar o LSB da soma).
     - Fim: `0x7E`
2. **Integração com RFID WL-134:**
   - **Serial:** PA9 (TX) e PA10 (RX). Baud rate de 9600 bps (8N2).
   - **Controle de Energia:** PB4 (Nível Lógico ALTO = Ligado, Nível Lógico BAIXO = Desligado).
   - **Protocolo (Frame WL-134 ASCII):**
     - Início: `0x02`
     - Dados (ASCII): 10 bytes HEX do cartão (LSB first), 4 bytes HEX do país, 1 byte Data flag, 1 byte Animal flag, 2 bytes reservados.
     - Checksum: 2 bytes ASCII HEX XOR.
     - Fim: `0x03`
3. **Monitoramento de Bateria:**
   - **Pino:** PB1.
   - **Hardware:** Circuito resistivo (Divisor de tensão com R1 = 100kΩ e R2 = 10kΩ).
   - **Lógica:** Ler valor analógico, converter para tensão real usando a proporção do divisor.
4. **Comunicação com o ESP32:**
   - **Serial:** PA2 (TX) e PA3 (RX).
   - **Objetivo:** Enviar os UUIDs lidos pelas tags e status da bateria para o ESP32 para processamento posterior.

## Diretrizes de Implementação
- Utilizar bibliotecas padrão da ST (HAL ou LL) apropriadas para a família STM32G0.
- Lidar com as recepções seriais de preferência utilizando interrupções (Interrupts) ou DMA para evitar perda de dados das tags.
- Garantir que as lógicas de acionamento de energia (PB4 e PB5) respeitem os tempos de inicialização (boot time) de cada módulo RFID antes de iniciar a leitura serial.
- Adotar as boas práticas definidas pelo `c_best_practices_agent.md`.
