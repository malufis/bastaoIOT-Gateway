# Protocolos de Comunicação RFID: Guia Técnico para Agentes de IA

Este documento reúne as especificações técnicas, formatos de pacotes, equações de soma de verificação e lógicas de decodificação dos leitores de RFID WL-134 e YRM100. Destina-se ao consumo exclusivo por agentes autônomos para desenvolvimento de drivers.

---

## 1. Módulo Leitor RFID WL-134 (Low Frequency - 134.2 KHz)

O leitor WL-134 opera na frequência de 134.2 KHz sob o protocolo FDX-B (ISO11784/5 - Animal Tag).

### 1.1. Configuração da USART (Host)
* **Baud Rate**: 9600 bps
* **Word Length**: 8 bits
* **Parity**: Nenhuma (None)
* **Stop Bits**: 2 bits (**8N2**)
* **Nível de Sinal**: TTL (5V, requer divisor ou tolerância no pino RX da MCU se operar em 3.3V)

### 1.2. Estrutura do Pacote de Dados (30 Bytes Brutos)
Toda tag lida com sucesso dispara a transmissão de um frame de 30 caracteres ASCII legíveis:

| Pos. | Campo | Tamanho | Descrição |
| :---: | :--- | :---: | :--- |
| **0** | **Start Byte** | 1 byte | Fixo: `0x02` (STX - Start of Text) |
| **1-10** | **Código do Cartão** | 10 bytes | String ASCII Hexadecimal, caracteres invertidos (**LSB First**) |
| **11-14** | **Código do País** | 4 bytes | String ASCII Hexadecimal, caracteres invertidos (**LSB First**) |
| **15** | **Data Flag** | 1 byte | ASCII `'0'` ou `'1'` |
| **16** | **Animal Flag** | 1 byte | ASCII `'0'` ou `'1'` (Indica se a tag é de uso animal) |
| **17-26** | **Reservado** | 10 bytes | ASCII `'0'` (Normalmente preenchido com zeros) |
| **27** | **Checksum** | 1 byte | XOR de todos os 26 caracteres ASCII das posições 1 a 26 |
| **28** | **Checksum Invertido** | 1 byte | Operação NOT lógica sobre o valor do Checksum (Posição 27) |
| **29** | **End Byte** | 1 byte | Fixo: `0x03` (ETX - End of Text) |

### 1.3. Algoritmo de Decodificação do Código da Tag (LSB First)
A representação hexadecimal da tag no frame é transmitida com a ordem dos caracteres invertida.

#### Exemplo Prático de Conversão:
* **Frame Bruto Recebido (Hex de caracteres ASCII)**:
  `02` `31 37 31 41 39 32 35 33 41 33` `34 38 33 30` `30` `31` `30 30 30 30 30 30 30 30 30 30` `07` `F8` `03`

* **Passo 1: Extrair e converter os bytes em caracteres ASCII**:
  * Código do Cartão (10 bytes): `31 37 31 41 39 32 35 33 41 33` $\rightarrow$ `"171A9253A3"`
  * Código do País (4 bytes): `34 38 33 30` $\rightarrow$ `"4830"`
  * Data Flag: `"0"`, Animal Flag: `"1"`
  * Reservados: `"0000000000"`

* **Passo 2: Reverter a ordem caractere a caractere (LSB First $\rightarrow$ MSB First)**:
  * String do Cartão `"171A9253A3"` invertida $\rightarrow$ `"3A3529A171"`
  * String do País `"4830"` invertida $\rightarrow$ `"0384"`

* **Passo 3: Converter de Hexadecimal para Decimal**:
  * Código do Cartão: `0x3A3529A171` $\rightarrow$ `250000023921` (Decimal)
  * Código do País: `0x0384` $\rightarrow$ `900` (Decimal)

* **Resultado da Tag Animal**: Código do País `900`, Código do Cartão `250000023921`.
  * Representação final impressa na tag física: `900250000023921`

### 1.4. Lógica de Validação do Checksum (XOR)
O byte 27 do frame é o resultado de uma operação XOR bit a bit entre os valores decimais dos caracteres ASCII de dados.
```c
uint8_t checksum = 0;
for (int i = 1; i <= 26; i++) {
    checksum ^= frame[i]; // XOR sobre os caracteres ASCII
}
if (checksum == frame[27] && (uint8_t)(~checksum) == frame[28]) {
    // Pacote é válido!
}
```

---

## 2. Módulo Leitor RFID YRM100 (Ultra High Frequency - UHF)

O YRM100 opera na faixa de UHF (EPC Class 1 Gen 2 / ISO18000-6C).

### 2.1. Configuração da USART (Host)
* **Baud Rate**: 115200 bps
* **Word Length**: 8 bits
* **Parity**: Nenhuma (None)
* **Stop Bits**: 1 bit (**8N1**)
* **Nível de Sinal**: TTL (3.3V)

### 2.2. Estrutura do Frame de Comando e Resposta (Binário)

| Byte | Campo | Tamanho | Descrição |
| :---: | :--- | :---: | :--- |
| **0** | **Header** | 1 byte | Fixo: `0xBB` |
| **1** | **Type** | 1 byte | Fluxo: `0x00` (Cmd do Host), `0x01` (Resp do Leitor), `0x02` (Notificação/Aviso) |
| **2** | **Cmd** | 1 byte | Código de comando/operação |
| **3** | **PL (MSB)** | 1 byte | Byte mais significativo do tamanho do parâmetro |
| **4** | **PL (LSB)** | 1 byte | Byte menos significativo do tamanho do parâmetro |
| **5 a (5+N)** | **Parameter** | N bytes | Payload de dados do frame (onde $N = \text{PL}$) |
| **6+N** | **Checksum** | 1 byte | Soma de verificação de integridade |
| **7+N** | **End Byte** | 1 byte | Fixo: `0x7E` |

### 2.3. Lógica do Checksum
A soma de validação é a soma aritmética simples de todos os bytes a partir do byte de **Type (Byte 1)** até o último byte do **Parameter (Byte 5+N)**, extraindo o byte menos significativo (LSB) do resultado (operação módulo 256).

$$\text{Checksum} = \left( \text{Type} + \text{Cmd} + \text{PL}_{\text{MSB}} + \text{PL}_{\text{LSB}} + \sum_{i=0}^{N-1} \text{Parameter}_i \right) \pmod{256}$$

#### Exemplo de Cálculo (Comando de Inventário Único):
* **Frame**: `BB 00 22 00 00 22 7E`
* **Bytes calculados**: `0x00` (Type) + `0x22` (Cmd) + `0x00` (PL_MSB) + `0x00` (PL_LSB) = `0x22`
* **Checksum**: `0x22`

### 2.4. Resumos de Comandos e Notificações Críticas

#### 2.4.1. Get Module Info (`0x03`)
* **Comando do Host**: `BB 00 03 00 01 00 04 7E`
* **Resposta (Exemplo)**: `BB 01 03 00 10 00 4D 31 30 30 20 32 36 64 42 6D 20 56 31 2E 30 22 7E` (Parâmetro `"M100 26dBm V1.0"`)

#### 2.4.2. Single Inventory (`0x22`)
* **Comando do Host**: `BB 00 22 00 00 22 7E`
* **Notificação de Tag Encontrada (Type = `0x02`)**:
  * **Frame Exemplo**: `BB 02 22 00 11 C9 34 00 30 75 1F EB 70 5C 59 04 E3 D5 0D 70 3A 76 EF 7E`
  * **Tamanho do Parâmetro (PL)**: `0x0011` (17 bytes em decimal).
  * **Parâmetros extraídos**:
    * RSSI (1 byte): `0xC9` (Calculado como complemento de dois $\rightarrow$ $-55$ dBm).
    * PC (2 bytes): `0x3400`.
    * **EPC (PL - 5 bytes = 12 bytes)**: `30 75 1F EB 70 5C 59 04 E3 D5 0D 70` (Código identificador da tag RFID UHF).
    * CRC do transponder (2 bytes): `0x3A76`.

#### 2.4.3. Set TX Power (`0xB6`)
* **Comando do Host (Ajuste para 20.00 dBm / 2000 em decimal $\rightarrow$ `0x07D0` hex)**:
  `BB 00 B6 00 02 07 D0 8F 7E`
* **Resposta (Sucesso)**: `BB 01 B6 00 01 00 B8 7E`
