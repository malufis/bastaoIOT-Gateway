@echo off
REM Compile ESP32 Bastão-ESP Firmware

echo ================================================
echo   Compilando Bastão-ESP Firmware
echo ================================================

REM Ativa ESP-IDF v5.4.1
echo.
echo [1/4] Ativando ESP-IDF v5.4.1...
call C:\Users\rlmal\esp\v5.4.1\esp-idf\export.bat
if errorlevel 1 (
    echo ERRO: Falha ao ativar ESP-IDF
    echo Execute manualmente: C:\Users\rlmal\esp\v5.4.1\esp-idf\install.bat
    pause
    exit /b 1
)

REM Vai para o diretorio do firmware
cd /d D:\git\Bastao\Bastão-ESP\esp32_firmware

REM Configura target ESP32
echo.
echo [2/4] Configurando target ESP32...
idf.py set-target esp32
if errorlevel 1 (
    echo ERRO: Falha ao configurar target
    pause
    exit /b 1
)

REM Compila o projeto
echo.
echo [3/4] Compilando projeto...
idf.py build
if errorlevel 1 (
    echo ERRO: Falha na compilacao
    pause
    exit /b 1
)

echo.
echo [4/4] Compilacao concluida!
echo.
echo Arquivos gerados em: build\Bastão-ESP.bin
echo.
echo Para fazer flash: idf.py -p COM3 flash
echo Para monitorar:   idf.py -p COM3 monitor
echo.

pause