# STM32G070CBTx Firmware Agent

Este agente é responsável por desenvolver, revisar e dar manutenção no firmware embarcado do microcontrolador **STM32G070CBTx** que atua como hub de sensoriamento e controle do Bastão-ESP.

---

## 1. Mapeamento de Hardware e Periféricos

Ao atuar neste projeto, o agente deve garantir a correta configuração dos seguintes recursos:

* **USART1 (PA9/PA10)**:
  * Conectado ao leitor de tag animal WL-134.
  * Configuração: **9600 bps, 8 data bits, no parity, 2 stop bits (8N2)**.
* **USART4 (PA0/PA1)**:
  * Conectado ao leitor de tag UHF YRM100.
  * Configuração: **115200 bps, 8 data bits, no parity, 1 stop bit (8N1)**.
* **USART2 (PA2/PA3)**:
  * Conectado ao canal de comunicação serial com o ESP32.
  * Configuração: **115200 bps, 8 data bits, no parity, 1 stop bit (8N1)**.
* **ADC1 (PB1/IN9)**:
  * Conectado ao circuito divisor de tensão da bateria (R1 = 100kΩ, R2 = 10kΩ).
  * Configuração de 12 bits de resolução.
* **GPIO PB4**:
  * Saída digital de controle de energia (Power Enable) do WL-134. Nível Lógico ALTO habilita o regulador/módulo.
* **GPIO PB5**:
  * Saída digital de controle de energia (Power Enable) do YRM100. Nível Lógico ALTO habilita o módulo.

---

## 2. Requisitos e Diretrizes do Firmware

* **Não Bloqueante (Non-blocking)**: O loop principal do sistema não deve conter delays (`HAL_Delay`) que afetem o tempo de resposta ou a amostragem de dados das seriais.
* **UART por Interrupção / DMA**:
  * Implementar obrigatoriamente a recepção de bytes nas portas USART1 e USART4 por interrupção (`HAL_UART_RxCpltCallback`) ou DMA circular.
  * Armazenar os dados em estruturas de buffer circular (`RFID_Buffer_t`) seguras contra condições de corrida (race conditions).
* **Filtro do ADC de Bateria**:
  * As leituras da bateria no pino PB1 devem passar por um filtro digital de média móvel (mínimo de 8 amostras) para atenuar variações rápidas provocadas por picos de consumo dos módulos RFID.
  * Aplicar a fórmula de conversão:
    $$\text{Tensão} = \left(\frac{\text{Valor ADC} \times 3.3}{4095}\right) \times 11.0$$
* **Formatos de Saída (JSON)**:
  * O STM32 deve traduzir as tags e dados de telemetria em strings JSON válidas e transmiti-las via USART2 para o ESP32, delimitadas por quebra de linha (`\n`):
    * Tag UHF: `{"type":"rfid","model":"YRM100","tag":"HEX"}\n`
    * Tag Animal: `{"type":"rfid","model":"WL-134","tag":"DECIMAL"}\n`
    * Bateria: `{"type":"batt","volt":XX.XX}\n`

---

## 3. Diretrizes de Codificação em C

* Adotar as boas práticas definidas pelo `c_best_practices_agent.md`.
* Utilizar o HAL da STMicroelectronics (`stm32g0xx_hal.h`).
* Organizar as funções de inicialização geradas pelo STM32CubeMX mantendo o código customizado estritamente dentro das tags `/* USER CODE BEGIN ... */` e `/* USER CODE END ... */`.
* Comentar de forma clara a lógica de tratamento de ponteiros `head` e `tail` do buffer circular.
