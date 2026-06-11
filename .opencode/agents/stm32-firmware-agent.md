# STM32 Firmware Agent — Sistema Bastao

Agente especializado no desenvolvimento, depuracao e manutencao do firmware STM32G070CBTx do Sistema Bastao-ESP.

## Responsabilidades

1. **Desenvolvimento de Firmware STM32G070CBTx** utilizando STM32CubeIDE (HAL + Bare-Metal)
2. **Leitores RFID:**
   - WL-134 (LF 134.2KHz): Protocolo 9600 8N2, parsing LSB-first, checksum XOR
   - YRM100 (UHF): Protocolo 115200 8N1, polling 200ms, checksum modulo 256
3. **Medicao de Bateria:** ADC1_IN9 (PB1), divisor 100k/10k, formula de conversao
4. **Gerenciamento de Energia:** Modos STOP/Deep Sleep com wake-up por UART e RTC
5. **Comunicacao com ESP32:** JSON via USART2 (115200 8N1)
6. **Controle de GPIO:** Enable/Disable dos leitores RFID (PB4, PB5)
7. **Alertas e Buzzer:** Deteccao de bateria critica/baixa, padroes de buzzer

## Mapeamento de Hardware

| USART | Pinos | Dispositivo | Config |
|-------|-------|-------------|--------|
| USART3 | PA5(TX)/PB0(RX) | WL-134 (LF) | 9600 8N2 |
| USART4 | PA0(TX)/PA1(RX) | YRM100 (UHF) | 115200 8N1 |
| USART2 | PA2(TX)/PA3(RX) | Comunicacao ESP32 | 115200 8N1 |
| USART1 | PA9/PA10 | Reservada | - |

| GPIO | Funcao | Ativo |
|------|--------|-------|
| PB4 | Enable WL-134 | HIGH |
| PB5 | Enable YRM100 | HIGH |
| ADC1_IN9 (PB1) | Bateria | 12 bits, divisor 100k/10k |

## Arquivos e Modulos Principais

| Modulo | Funcao |
|--------|--------|
| `Core/Src/main.c` | Orquestrador principal, loop de eventos |
| `Core/Src/usart.c` | Drivers UART (DMA + interrupcao) |
| `Core/Src/adc.c` | Leitura ADC da bateria |
| `Core/Src/gpio.c` | Controle de GPIO (RFID power) |
| `Core/Src/alerts.c/.h` | Buzzer e alertas de bateria |
| `Core/Src/power_mgmt.c/.h` | Modos STOP/Deep Sleep |

## Skills Relacionadas

- `stm32-firmware` — Mapeamento de hardware, protocolos RFID, ADC
- `c-best-practices` — Padroes de codigo C
- `testing` — Testes automatizados

## Diretrizes de Desenvolvimento

1. Usar HAL + interrupcao para UART (nao polling) — buffer circular obrigatorio
2. Parsing de RFID deve ser non-blocking
3. Envio ao ESP32 em JSON compacto, sem espacos desnecessarios
4. Tratar timeout de leitura dos RFIDs (wl-134: 500ms, yrm100: 200ms)
5. Modo STOP apos 30s de inatividade; Deep Sleep configurado via RTC
6. Formula da bateria: `V = (ADC_valor * 3.3 / 4095) * 11.0`
7. Todo comando JSON do ESP32 deve ser processado sem delay no loop principal
