# RFID & Sensor Agent

Este agente é responsável por desenvolver e manter o firmware associado aos leitores RFID (WL-134 e YRM100) no microcontrolador STM32G070CBTx, garantindo a integridade dos dados e o gerenciamento de energia apropriado.

---

## 1. Responsabilidades Principais

### 1.1. Integração com o Leitor RFID YRM100 (UHF)
* **Conexão Física**: USART4 (Pinos PA0-TX / PA1-RX).
* **Parâmetros Serial**: 115200 bps, 8 data bits, no parity, 1 stop bit (**8N1**).
* **Alimentação**: Controlada via pino **PB5** (Alto = Ligado, Baixo = Desligado).
* **Tratamento de Protocolo**:
  * Seguir estritamente o manual de frames binários: `Header (0xBB)`, `Type`, `Cmd`, `PL (2 bytes)`, `Parameter (PL bytes)`, `Checksum`, `End (0x7E)`.
  * **Checksum**: Soma aritmética do byte `Type` até o último byte de `Parameter` módulo 256.
  * **Extração de EPC**: Em eventos de notificação de tag (`Cmd 0x22`), extrair o EPC a partir do offset apropriado no array de parâmetros (ignorando RSSI e PC).
  * Consultar detalhes matemáticos e exemplos em: [rfid_protocols.md](file:///d:/git/Bastao/Bastão-ESP/aprendizado/rfid_protocols.md#2-módulo-leitor-rfid-yrm100-ultra-high-frequency---uhf).

### 1.2. Integração com o Leitor RFID WL-134 (LF 134.2 KHz)
* **Conexão Física**: USART3 (Pinos PA5-TX / PB0-RX).
* **Parâmetros Serial**: 9600 bps, 8 data bits, no parity, 2 stop bits (**8N2**).
* **Alimentação**: Controlada via pino **PB4** (Alto = Ligado, Baixo = Desligado).
* **Tratamento de Protocolo**:
  * Receber pacotes ASCII de 30 bytes iniciando com `0x02` (STX) e terminando com `0x03` (ETX).
  * **Algoritmo de Conversão LSB-First**:
    1. Isolar os 10 caracteres do Código do Cartão (Card Number) e 4 caracteres do Código do País (Country Code).
    2. Reverter caractere a caractere ambas as strings hexadecimais para restaurar a ordem MSB-First.
    3. Converter as strings hexadecimais resultantes para seus equivalentes decimais.
  * **Checksum**: XOR cumulativo dos caracteres ASCII das posições 1 a 26 do frame. Validar contra o byte 27 do frame e seu inverso no byte 28.
  * Consultar detalhes matemáticos e exemplos em: [rfid_protocols.md](file:///d:/git/Bastao/Bastão-ESP/aprendizado/rfid_protocols.md#1-módulo-leitor-rfid-wl-134-low-frequency---1342-khz).

---

## 2. Boas Práticas de Implementação

* **Recepção UART Assíncrona**: O agente deve implementar recepção por interrupções (`HAL_UART_Receive_IT`) ou DMA, armazenando os bytes recebidos em buffers circulares dedicados. Evite o uso de funções bloqueantes como `HAL_UART_Receive` com timeouts longos.
* **Energização Sequencial**: Ao ligar os módulos (PB4 e PB5), garanta um delay de boot apropriado antes de iniciar a leitura serial dos leitores.
* **Prevenção de Perda de Pacotes**: As funções de parsing dos buffers circulares devem rodar no loop principal de forma cooperativa, consumindo pacotes válidos e limpando o buffer de dados corrompidos.
* **Padrão de Código**: Seguir as regras descritas em `c_best_practices_agent.md`.
