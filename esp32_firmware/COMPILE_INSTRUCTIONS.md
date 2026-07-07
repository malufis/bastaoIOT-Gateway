# Compilação Bastão-ESP

## Pré-requisito: Gerar Configuração

Antes de compilar, edite `private_configs.env` com as credenciais do ambiente
e gere o header de configuração:

```cmd
cd D:\git\Bastao\Bastao-ESP\esp32_firmware

:: Edite o arquivo .env com suas credenciais
notepad private_configs.env

:: Gera main/private_configs.h a partir do .env
python generate_config.py
```

**Variáveis principais no .env:**

| Variável | Descrição | Exemplo |
|----------|-----------|---------|
| `BASTAO_MQTT_URI` | Broker MQTT | `mqtt://209.50.240.55:1883` |
| `BASTAO_APN_NAME` | APN 4G | `iot.datatem.com.br` |
| `BASTAO_WIFI_SSID` | Wi-Fi SSID (fallback) | `""` (vazio = desligado) |
| `BASTAO_AES_KEY` | Chave AES-256 (64 hex chars) | `0123456789...` |
| `BASTAO_NET_MODE` | Modo de rede | `auto`, `wifi_only`, `cellular_only` |

---

## Problema de Dependências

O ambiente ESP-IDF tem um conflito de versões com `pydantic`. Para compilar, use uma das opções abaixo:

---

## Opção 1: Compilação Manual (Recomendado)

Abra um **CMD.exe** (não PowerShell) e execute:

```cmd
cd D:\git\Bastao\Bastão-ESP\esp32_firmware
C:\Espressif\frameworks\esp-idf-v5.5.2\export.bat
idf.py set-target esp32s3
idf.py build
```

---

## Opção 2: Scripts Disponíveis

Execute diretamente no CMD:

```cmd
D:\git\Bastao\Bastão-ESP\esp32_firmware\compile.bat
```

O script `compile.bat` inclui a geração de config automaticamente.
Para bypass de dependências quebradas, use `compile_bypass.bat`.

---

## Opção 3: VS Code / Espressif IDE

1. Abra o diretório `D:\git\Bastao\Bastão-ESP\esp32_firmware` no VS Code
2. Instale a extensão **Espressif IDF**
3. Configure o path do ESP-IDF: `C:\Espressif\frameworks\esp-idf-v5.5.2`
4. Execute `python generate_config.py` para gerar config
5. Execute `idf.py build` pelo terminal da extensão

---

## Saída Esperada

```
-- Project name: k10_mesh_coordinator
-- Building for target esp32s3
-- ccache will be used
...
[100/100] Linking ES...
Successfully created esp32s3 image.
```

---

## Flash do Firmware

```cmd
idf.py -p COM3 flash monitor
```

(Substitua `COM3` pela porta correta do seu ESP32)

> **Nota:** O modem SIMCom 7663E consome picos de corrente de até 2A.
> Use fonte externa 5V/3A. USB de PC pode causar brownout detector.

---

## Arquivo de Compilação Gerado

- `build/k10_mesh_coordinator.bin` - Firmware principal (~1.6MB)
- `build/bootloader/bootloader.bin` - Bootloader
- `build/partition-table.bin` - Tabela de partições

**Nota:** Flash configurada para 16MB. Partições: OTA 4MB cada, SPIFFS 2MB.

---

## Configuração EMQX (sistemaBastao)

O ESP32 conecta no broker MQTT com autenticação dinâmica:

- **Username:** MAC do ESP32 (gerado em runtime)
- **Password:** Chave AES em hexadecimal (64 chars)

O bastão deve estar previamente cadastrado na tabela `bastoes` do
sistemaBastao com `numero_serie` = MAC e `chave_aes` = mesma chave do .env.
