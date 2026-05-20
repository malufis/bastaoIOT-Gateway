# Compilação Bastão-ESP

## Problema de Dependências

O ambiente ESP-IDF tem um conflito de versões com `pydantic`. Para compilar, use uma das opções abaixo:

---

## Opção 1: Compilação Manual (Recomendado)

Abra um **CMD.exe** (não PowerShell) e execute:

```cmd
cd D:\git\Bastao\Bastão-ESP\esp32_firmware
C:\Users\rlmal\esp\v5.4.1\esp-idf\export.bat
idf.py set-target esp32
idf.py build
```

---

## Opção 2: Scripts Disponíveis

Execute diretamente no CMD:
```cmd
D:\git\Bastao\Bastão-ESP\esp32_firmware\compile.bat
```

---

## Opção 3: VS Code / Espressif IDE

1. Abra o diretório `D:\git\Bastao\Bastão-ESP\esp32_firmware` no VS Code
2. Instale a extensão **Espressif IDF**
3. Configure o path do ESP-IDF: `C:\Users\rlmal\esp\v5.4.1\esp-idf`
4. Execute `idf.py build` pelo terminal da extensão

---

## Saída Esperada

```
-- Building for target esp32
-- ccache will be used
[1/100] Building C object esp-idf/esp_system/CMakeFiles/__idf_esp_system.dir/port/arch/xtensa/panic_reason.S.obj
...
[100/100] Linking ES...
[Bastão-ESP.bin] Built successfully
```

---

## Flash do Firmware

```cmd
idf.py -p COM3 flash monitor
```

(Substitua `COM3` pela porta correta do seu ESP32)

---

## Arquivo de Compilação Gerado

- `build/Bastão-ESP.bin` - Firmware principal
- `build/bootloader/bootloader.bin` - Bootloader
- `build/partitions.bin` - Tabela de partições