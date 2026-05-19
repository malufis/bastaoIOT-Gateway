# Documentação do Projeto: Sistema Bastão-ESP

## 1. Visão Geral
Este projeto consiste em um sistema de sensoriamento e conectividade baseado em uma arquitetura de múltiplos microcontroladores (ESP32 e STM32) para leitura de tags RFID, processamento de dados e transmissão para nuvem e endpoints locais.

## 2. Arquitetura de Hardware

### 2.1. Módulo de Sensoriamento (STM32G070CBTx)
Responsável por toda a interface física com os sensores e gerenciamento de energia.
- **Microcontrolador:** STM32G070CBTx.
- **Periféricos:**
  - **USART1 (PA9/PA10):** Interface com Leitor RFID WL-134 (9600 bps, 8N2).
  - **USART2 (PA2/PA3):** Interface de comunicação com o ESP32 (UART TTL).
  - **USART4 (PA0/PA1):** Interface com Leitor RFID YRM100 (115200 bps, 8N1).
  - **ADC1 (PB1/IN9):** Monitoramento de tensão da bateria (Divisor resistivo: R1=100k, R2=10k).
  - **GPIO Output (PB4):** Controle de energia do WL-134 (High=On).
  - **GPIO Output (PB5):** Controle de energia do YRM100 (High=On).

### 2.2. Módulo de Conectividade (ESP32)
Responsável pela lógica de rede, criptografia e integração com a nuvem.
- **Microcontrolador:** ESP32 (usando ESP-IDF v5.x e FreeRTOS).
- **Conectividade Local:** BLE MESH (Bluetooth Low Energy Mesh).
  - **Papel:** Coordenador/Provisionador.
  - **Endpoint:** Tela K10.
  - **Segurança:** Provisionamento fechado via Whitelist de UUIDs e chaves OOB fixas.
- **Conectividade Nuvem:** SIMCom 7663E (4G/LTE + GPS).
  - **Interface:** UART (Pinos 17 e 18).
  - **Protocolo de Rede:** PPP (Point-to-Point Protocol) para ativação de dados.
  - **Aplicação:** Cliente MQTT operando sobre a pilha PPP.
- **Localização:** GPS/GLONASS via SIMCom 7663E.

## 3. Protocolos de Comunicação

### 3.1. RFID YRM100 (UHF)
- **Protocolo de Frame:**
  - Header: `0xBB`
  - Type: `0x00` (Cmd), `0x01` (Resp), `0x02` (Notice)
  - Checksum: Soma simples do byte Type até o último byte de parâmetro.
  - End: `0x7E`

### 3.2. RFID WL-134 (Animal Tag - 134.2KHz)
- **Protocolo de Frame (ASCII):**
  - Start: `0x02`
  - Data: 10 bytes HEX do cartão + 4 bytes país + flags.
  - End: `0x03`

### 3.3. Mensageria e Segurança
- **Formato de Dados:** JSON.
- **Criptografia:** AES (padrão ESP32) para todos os dados enviados à tela ou servidor.
- **MQTT:** Configurações e comandos trafegam via MQTT criptografado.

## 4. Estrutura do Projeto
- `/stm32_firmware`: Projeto STM32CubeIDE contendo o firmware de sensoriamento.
- `/esp32_firmware`: Projeto ESP-IDF contendo o firmware de conectividade.
- `/docs`: Manuais técnicos e documentação adicional.
- `.agents/`: Instruções específicas para cada subsistema do projeto.

## 5. Fluxo de Operação
1. O STM32 ativa a energia dos módulos RFID.
2. Ao detectar uma tag, o STM32 faz o parsing do UUID e envia para o ESP32 via UART.
3. O ESP32 empacota a UUID, dados de bateria e localização (GPS) em um JSON.
4. O payload é criptografado com AES.
5. O dado é enviado para a Tela K10 via BLE Mesh e para a nuvem via MQTT (4G).
## 6. Referências Técnicas
Os manuais originais com os protocolos completos estão localizados na raiz do projeto:

- **YRM100 UHF Reader:** [Communication user Protocol V2.1_en.docx](file:///d:/git/Bastao/Bast%C3%A3o-ESP/Communication%20user%20Protocol%20V2.1_en.docx)
- **WL-134 Animal Tag Reader:** [RFID reader module.pdf](file:///d:/git/Bastao/Bast%C3%A3o-ESP/RFID%20reader%20module.pdf)

---
*Nota: Esta documentação foi gerada automaticamente e deve ser atualizada conforme o progresso da implementação.*
