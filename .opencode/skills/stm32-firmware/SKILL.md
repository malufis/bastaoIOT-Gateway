---
name: stm32-firmware
description: Mapeamento de hardware do STM32G070CBTx, protocolos de leitores RFID (WL-134 e YRM100), ADC da bateria e arquiteturas non-blocking com buffer circular.
---

# STM32 Firmware Skill

Esta skill rege o desenvolvimento, revisao e manutencao do firmware embarcado do microcontrolador **STM32G070CBTx** que atua como hub de sensoriamento e controle do Bastao-ESP.

---

## 1. Mapeamento de Hardware e Perifericos

Ao atuar neste projeto, deve-se garantir a correta configuracao dos seguintes recursos:

### 1.1. Interfaces Seriais (UART)

| USART | Pinos | Dispositivo | Configuracao |
|-------|-------|-------------|--------------|
| USART3 | PA5 (TX) / PB0 (RX) | Leitor WL-134 (LF) | 9600 bps, **8N2** |
| USART4 | PA0 (TX) / PA1 (RX) | Leitor YRM100 (UHF) | 115200 bps, **8N1** |
| USART2 | PA2 (TX) / PA3 (RX) | Comunicacao com ESP32 | 115200 bps, **8N1** |
| USART1 | PA9 / PA10 | Nao Utilizada / Reservada | - |

### 1.2. Conversor Analogico-Digital (ADC)

| Canal | Pino | Funcao | Configuracao |
|-------|------|--------|---------------|
| ADC1_IN9 | PB1 | Leitura de tensao da bateria | 12 bits, divisor 100k/10k |

**Formula de Conversao:**
$$\text{Tensao} = \left(\frac{\text{Valor ADC} \times 3.3}{4095}\right) \times 11.0$$

### 1.3. Controle de Energia (GPIO Output)

| Pino | Funcao | Ativo | Inativo |
|------|--------|-------|---------|
| PB4 | Enable WL-134 | HIGH | LOW |
| PB5 | Enable YRM100 | HIGH | LOW |

---

## 2. Protocolos RFID

### 2.1. Leitor YRM100 (UHF - ISO18000-6C / EPC Class 1 Gen 2)

* **Conexao:** USART4 (PA0/PA1)
* **Baud Rate:** 115200 bps, 8N1
* **Alimentacao:** Controlada via pino PB5

**Protocolo de Frame Binario:**
```
| Header | Type | Cmd | PL (2 bytes) | Parameter (PL bytes) | Checksum | End |
|  0xBB  | 1B  | 1B  |     2B       |       PL bytes       |   1B    | 0x7E |
```

* **Header:** `0xBB` fixo
* **Type:** `0x00` (Comando), `0x01` (Resposta), `0x02` (Notificacao)
* **PL:** Comprimento dos parametros (2 bytes, little-endian)
* **Checksum:** Soma aritmetica do byte Type ate o ultimo byte de Parameter, modulo 256
* **End:** `0x7E` fixo

**Comando de Notificacao de Tag:** `Cmd = 0x22`
* O EPC esta presente no campo Parameter (ignorar bytes de RSSI e PC)

**Exemplo de Extracao:**
```
Frame: BB 02 22 07 00 30 75 1F EB 70 5C 59 04 E3 D5 0D 70 F5 7E
PL = 0x0007 (7 bytes)
Parameter: 30 75 1F EB 70 5C 59 04 (EPC: 30751FEB705C5904)
E3 = checksum (Type(02)+Cmd(22)+PL(07)+Parameter = 0xE3)
```

### 2.2. Leitor WL-134 (LF - 134.2 KHz / ISO11784/5 FDX-B)

* **Conexao:** USART3 (PA5/PB0)
* **Baud Rate:** 9600 bps, 8N2
* **Alimentacao:** Controlada via pino PB4

**Protocolo de Frame ASCII (30 bytes):**
```
| STX | Card Number (10) | Country (4) | Flags (2) | Checksum (2) | ETX |
| 0x02|   10 caracteres  |   4 chars  |   2 chars |    2 chars  | 0x03|
```

**Algoritmo de Conversao LSB-First:**
1. Isolar os 10 caracteres do Codigo do Cartao (bytes 1-10)
2. Isolar os 4 caracteres do Codigo do Pais (bytes 11-14)
3. **Inverter** cada string caractere a caractere (LSB → MSB)
4. Converter as strings hexadecimais invertidas para decimal

**Checksum:** XOR cumulativo dos caracteres ASCII das posicoes 1-26.
* Validar contra o byte 27 e seu inverso no byte 28.

**Exemplo:**
```
Raw: 30 30 32 35 30 30 30 30 30 32 (Card Number)
Invertido: 32 30 30 30 30 30 30 32 35 30
Decimal: 900250000023
```

---

## 3. Requisitos e Diretrizes do Firmware

### 3.1. Arquitetura Non-Blocking

* O loop principal (`while(1)`) **nao deve** conter delays (`HAL_Delay`) que afetem o tempo de resposta.
* Toda recepcao serial deve ser feita por **interrupcao** (`HAL_UART_RxCpltCallback`) ou DMA circular.
* Armazenar dados em **buffers circulares** com gestao de ponteiros `head` e `tail` (operacao modulo).

### 3.2. Buffer Circular

```c
typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} CircularBuffer_t;
```

* `head`: posicao de escrita (incrementado ao receber byte)
* `tail`: posicao de leitura (incrementado ao consumir dado)
* Tamanho deve ser potencia de 2 para operacao modulo eficiente (ex: 256, 512)

### 3.3. Filtro Digital de Bateria

* Aplicar **media movel** de no minimo **8 amostras** para atenuar ruidos.
* Enviar telemetria a cada **5 segundos** via UART2 para ESP32.

### 3.4. Formato de Saida JSON

Todos os dados devem ser enviados via USART2 delimitados por `\n`:

```json
{"type":"rfid","model":"YRM100","tag":"30751FEB705C5904E3D50D70"}
{"type":"rfid","model":"WL-134","tag":"900250000023921"}
{"type":"batt","volt":8.45}
```

### 3.5. Diretrizes de Codificacao

* Usar biblioteca **STM32Cube HAL** (`stm32g0xx_hal.h`)
* Manter codigo customizado dentro das tags `/* USER CODE BEGIN ... */`
* Todas as funcoes devem ter documentacao Doxygen (@brief, @param, @return)
* Usar tipos de tamanho fixo (`uint8_t`, `uint16_t`, `int32_t`)
* Evitar alocacao dinamica (`malloc`) em rotinas de interrupcao

---

## 4. Estrutura de Arquivos Recomendada

```
stm32_firmware/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── rfid_parser.h          # Parser WL-134 e YRM100
│   │   ├── battery_monitor.h     # ADC e filtro de media movel
│   │   ├── circular_buffer.h     # Buffer circular generico
│   │   └── power_control.h       # Controle de energia PB4/PB5
│   └── Src/
│       ├── main.c                # Orquestracao (inicializacoes)
│       ├── rfid_parser.c
│       ├── battery_monitor.c
│       ├── circular_buffer.c
│       └── power_control.c
└── STM32G070CBTX_FLASH.ld
```
