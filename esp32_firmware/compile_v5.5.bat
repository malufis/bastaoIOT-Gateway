@echo off
REM Script de compilacao usando ESP-IDF v5.5.2 de C:\Espressif

echo ================================================
echo   Compilando Bastão-ESP Firmware
echo ================================================

REM Define paths
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.5.2
set IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.5_py3.11_env
set IDF_TOOLS_PATH=C:\Espressif

REM Adiciona ao PATH
set PATH=%IDF_PATH%;%IDF_PATH%\tools;%IDF_PYTHON_ENV_PATH%;%IDF_PYTHON_ENV_PATH%\Scripts;%IDF_TOOLS_PATH%\tools\xtensa-esp-elf\esp-14.2.0_20241119\xtensa-esp-elf\bin;%IDF_TOOLS_PATH%\tools\esp32ulp-elf\2.38_20240113;%PATH%

echo.
echo [INFO] ESP-IDF: %IDF_PATH%
echo [INFO] Python: %IDF_PYTHON_ENV_PATH%
echo.

REM Vai para o projeto
cd /d D:\git\Bastao\Bastão-ESP\esp32_firmware

REM Configura target se necessario
if not exist build\sdkconfig (
    echo [1/4] Configurando target ESP32...
    python %IDF_PATH%\tools\idf.py set-target esp32
) else (
    echo [INFO] Target ja configurado
)

REM Compila
echo.
echo [2/4] Compilando projeto...
python %IDF_PATH%\tools\idf.py build

echo.
echo [3/4] Compilacao concluida!
echo.
echo Para fazer flash: python %IDF_PATH%\tools\idf.py -p COM3 flash
echo Para monitorar:   python %IDF_PATH%\tools\idf.py -p COM3 monitor
echo.

pause