# Guia de Compilação do Projeto Bastão-ESP

Este guia orienta a configuração do ambiente e a compilação do firmware para os módulos do projeto Bastão-ESP.

---

## 1. Configuração do Ambiente ESP-IDF (Recomendado v5.5.2)

Para garantir a compatibilidade de APIs, drivers de rede (PPP/LwIP) e correto gerenciamento de memória, este projeto foi validado utilizando o **ESP-IDF v5.5.2**.

### Requisitos do compilador (Toolchain)
O compilador obrigatório e homologado para esta versão do ESP-IDF é:
- **xtensa-esp-elf GCC versão:** `esp-14.2.0_20251107`

Se outra versão da toolchain estiver ativa no seu ambiente, a compilação falhará com erro de verificação de versão (`tool_version_check.cmake`).

### Configuração no Windows (PowerShell / CMD)
Você pode usar o instalador online do ESP-IDF para obter a v5.5.2:
1. Baixe o instalador de: [Espressif Downloads](https://dl.espressif.com/dl/esp-idf/)
2. Instale o ESP-IDF v5.5.2 no diretório padrão (ex: `C:\Espressif\frameworks\esp-idf-v5.5.2`).
3. O projeto fornece um script automatizado [compile_v5.5.bat](file:///D:/git/Bastao/Bastao-ESP/esp32_firmware/compile_v5.5.bat) que já configura os caminhos do compilador e as variáveis de ambiente locais antes de chamar a compilação.

### Configuração no Linux/Mac (Bash)
```bash
# 1. Clone o ESP-IDF na versão v5.5.2
git clone -b v5.5.2 --recursive https://github.com/espressif/esp-idf.git C:/esp/esp-idf-v5.5.2
cd C:/esp/esp-idf-v5.5.2

# 2. Instale dependências e a toolchain
./install.sh

# 3. Ative o ambiente no terminal atual
source export.sh
```

---

## 2. Compilação do ESP32 (Bastão-ESP)

O firmware do ESP32 reside no diretório [esp32_firmware/](file:///D:/git/Bastao/Bastao-ESP/esp32_firmware).

### Compilação Automatizada (Windows)
A maneira recomendada de compilar no Windows é usar o script local do projeto:
```powershell
# Acesse a pasta do firmware
cd D:\git\Bastao\Bastao-ESP\esp32_firmware

# Execute o compilador
.\compile_v5.5.bat
```
Este script define as variáveis de ambiente corretas, aponta para a pasta da toolchain `esp-14.2.0_20251107` e executa o comando `idf.py build`.

### Compilação Manual (Linha de Comando)
Caso as variáveis já estejam exportadas no seu terminal:
```bash
cd esp32_firmware

# Define o chip de destino (caso seja a primeira build)
idf.py set-target esp32s3

# Compila o firmware
idf.py build

# Grava o firmware na porta serial (substitua COM7 pela sua porta física)
idf.py -p COM7 flash

# Abre o monitor de logs serial
idf.py -p COM7 monitor
```

---

## 3. Notas Importantes sobre o SDK Config e Hardware

Algumas configurações cruciais foram adicionadas ao [sdkconfig.defaults](file:///D:/git/Bastao/Bastao-ESP/esp32_firmware/sdkconfig.defaults) para evitar falhas físicas e lógicas:

### A. Tamanho da Memória Flash (4MB)
A tabela de partições do Bastão-ESP ([partitions.csv](file:///D:/git/Bastao/Bastao-ESP/esp32_firmware/partitions.csv)) foi estendida para **1600KB** nas partições de aplicativo redundantes (`ota_0`/`ota_1`) para acomodar as bibliotecas de BLE Mesh e rede celular. 
O tamanho total ocupado pelas partições é de aproximadamente **3.8MB**. Por isso, o tamanho padrão de flash do projeto foi alterado de 2MB para 4MB:
```ini
CONFIG_ESPTOOLPY_FLASHSIZE_4MB=y
CONFIG_ESPTOOLPY_FLASHSIZE="4MB"
```
*Se a gravação falhar ou o CMake alertar sobre o tamanho de flash excedido, certifique-se de que essas opções estão configuradas para 4MB.*

### B. Otimização de IRAM (Evitar estouro de memória de instruções)
Compilar o suporte a Wi-Fi, BLE e Pilha de Rede Celular consome muita memória IRAM. Por padrão, o ESP-IDF coloca funções críticas de Wi-Fi e PHY na IRAM para velocidade de execução, o que causa estouro do segmento `.iram0.text` (comum estourar em ~7KB a 10KB).
Para evitar esse erro de linkagem, desabilitamos essa otimização nas configurações:
```ini
CONFIG_ESP_WIFI_IRAM_OPT=n
CONFIG_ESP_WIFI_RX_IRAM_OPT=n
CONFIG_ESP_PHY_IRAM_OPT=n
```
Isso move as rotinas não críticas de Wi-Fi e PHY para a memória Flash externa, liberando cerca de 15KB na IRAM.

### C. Suporte a Conexão Celular (PPP)
O módulo de telefonia celular [simcom_ppp.c](file:///D:/git/Bastao/Bastao-ESP/esp32_firmware/main/simcom_ppp.c) integra-se à pilha TCP/IP do ESP-IDF. É necessário que o suporte a PPP esteja ativo no LwIP:
```ini
CONFIG_LWIP_PPP_SUPPORT=y
CONFIG_LWIP_PPP_PAP_SUPPORT=y
CONFIG_LWIP_PPP_CHAP_SUPPORT=y
```

---

## 4. Compilação do STM32 (Firmware)

O firmware do STM32 controla periféricos e estados de baixo consumo. Ele está em [stm32_firmware/](file:///D:/git/Bastao/Bastao-ESP/stm32_firmware).

### Usando STM32CubeIDE
1. Abra o STM32CubeIDE.
2. Importe o projeto `stm32_firmware` a partir do diretório raiz.
3. Clique em **Build > Build Project** para gerar o arquivo `.elf`/`.bin`.

### Usando make (CLI)
```bash
cd stm32_firmware
make
```

---

## 5. Compilação do K10 Firmware (ESP32-S3 Receptor)

O K10 atua como nó Mesh receptor. Ele reside em [k10_firmware/](file:///D:/git/Bastao/Bastao-ESP/k10_firmware).

```bash
cd k10_firmware

# Configure o target do chip para ESP32-S3
idf.py set-target esp32s3

# Compile o projeto
idf.py build
```

---

## 6. Testes Automatizados e Diagnósticos

O diretório [teste_automatizado/](file:///D:/git/Bastao/Bastao-ESP/teste_automatizado) oferece scripts Python de validação de campo:

```bash
cd teste_automatizado

# Testar configuração básica de rede
python verify_network_config.py

# Validar consumo e modos de sono
python verify_sleep_modes.py

# Teste de estresse de tráfego de dados na rede Mesh
python verify_mesh_stress.py

# Visualização de logs remotos (Telnet) do Bastão
python esp32_log_viewer.py <IP_DO_BASTAO>
```

---

## 7. Características GATT do Bastão-ESP (BLE)

UUIDs de comunicação para a aplicação móvel:

| UUID | Nome | Descrição |
|------|------|-----------|
| `0xFF01` | Config Write | Escrita de configurações enviadas pelo app |
| `0xFF02` | Config Read | Leitura de configurações do bastão |
| `0xFF03` | Business Write | Envio de dados e payloads de negócio |
| `0xFF04` | Last Tag | Notificação da última tag RFID lida |
| `0xFF05` | Device Status | Estado físico e bateria do bastão |
| `0xFF06` | Cellular Status | Status da conexão 4G/LTE e RSSI |
| `0xFF07` | Logger | Stream de logs remotos em tempo real |

---

## 8. Requisitos e Dependências do Sistema

### ESP32 (Bastão / K10)
- **Framework:** ESP-IDF v5.5.2
- **Python:** v3.8+ (v3.11 recomendado no ambiente virtual)
- **Ferramenta de Build:** CMake 3.16+ e Ninja
- **Biblioteca de Interface (K10):** LVGL 8.x
- **SDK de Comunicação:** ESP-BLE-Mesh

### STM32
- **IDE:** STM32CubeIDE 1.12+ ou Toolchain `arm-none-eabi-gcc`
- **Microcontrolador:** STM32G070CBTx