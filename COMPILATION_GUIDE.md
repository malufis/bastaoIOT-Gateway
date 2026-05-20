# Compilação do Projeto Bastão-ESP

## 1. Configuração do Ambiente ESP-IDF

### Windows (PowerShell)
```powershell
# 1. Instalar ESP-IDF 5.x
# Baixe de: https://dl.espressif.com/dl/esp-idf/espressif-esp-idf-5.3.1-windows64.zip

# 2. Extraia e execute o script de setup
cd C:\esp\esp-idf
.\install.ps1

# 3. Ative o ambiente
.\export.ps1
```

### Linux/Mac
```bash
# 1. Clone o ESP-IDF
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.3.1
git submodule update --init --recursive

# 2. Instale dependências
./install.sh

# 3. Ative o ambiente
source export.sh
```

## 2. Compilação do ESP32 (Bastão-ESP)

```bash
cd esp32_firmware

# Configure o projeto
idf.py set-target esp32
idf.py menuconfig

# Compile
idf.py build

# Flash (substitua /dev/ttyUSB0 pela porta correta)
idf.py -p /dev/ttyUSB0 flash monitor
```

## 3. Compilação do STM32 (Firmware)

### Usando STM32CubeIDE
1. Abra o STM32CubeIDE
2. Importe o projeto `stm32_firmware`
3. Build > Build Project

### Usando make (CLI)
```bash
cd stm32_firmware
make
```

## 4. Compilação do K10 Firmware

```bash
cd k10_firmware

# Configure o target
idf.py set-target esp32s3

# Compile
idf.py build
```

## 5. Arquivos Principais

### ESP32 (esp32_firmware/main/)
| Arquivo | Descrição |
|---------|-----------|
| main.c | Orquestrador principal |
| ble_mobile.c/.h | GATT Server para app mobile |
| mesh_coordinator.c/.h | Coordinator BLE Mesh |
| simcom_ppp.c/.h | Driver celular SIMCom 7663E |
| wifi_driver.c/.h | Driver Wi-Fi Station |
| esp32_logger.c/.h | Logging Telnet/BLE |
| esp_power.c/.h | Gerenciamento de energia |

### STM32 (stm32_firmware/Core/)
| Arquivo | Descrição |
|---------|-----------|
| Src/main.c | Main loop |
| Src/power_mgmt.c | Sleep modes |
| Src/alerts.c | Sistema de alertas |
| Inc/power_mgmt.h | API de energia |

### K10 (k10_firmware/)
| Arquivo | Descrição |
|---------|-----------|
| components/k10_mesh/k10_mesh_node.c | Nó Mesh receptor |
| main/k10_main_updated.c | Main do K10 |

## 6. Testes Automatizados

```bash
cd teste_automatizado

# Teste de configuração de rede
python verify_network_config.py

# Teste de modos de sono
python verify_sleep_modes.py

# Teste de estresse Mesh
python verify_mesh_stress.py

# Visualização de logs (com Bastão conectado via Wi-Fi)
python esp32_log_viewer.py <IP_DO_BASTAO>
```

## 7. Debug via Telnet

```bash
# Conectar ao Bastão via Telnet
telnet 192.168.1.100 23
```

## 8. Características GATT do Bastão-ESP

| UUID | Nome | Descrição |
|------|------|-----------|
| 0xFF01 | Config Write | Escrita de configuração (app → bastão) |
| 0xFF02 | Config Read | Leitura de configuração |
| 0xFF03 | Business Write | Dados de negócio |
| 0xFF04 | Last Tag | Última tag lida (notify) |
| 0xFF05 | Device Status | Status do dispositivo |
| 0xFF06 | Cellular Status | Status da rede celular |
| 0xFF07 | Logger | Stream de logs |

## 9. Requisitos de Dependências

### ESP-IDF
- esp-idf v5.x
- Python 3.8+
- CMake 3.16+
- ninja-build

### STM32
- STM32CubeIDE 1.12+ ou
- arm-none-eabi-gcc
- STM32G070CBTx

### K10 (ESP32-S3)
- ESP-IDF 5.x
- LVGL 8.x
- ESP-BLE-Mesh SDK