@echo off
REM Script de compilacao unificado usando ESP-IDF v5.5.2 de C:\Espressif

echo ================================================
echo   Compilando Bastao-ESP Firmware (ESP-IDF v5.5.2)
echo ================================================

REM Define paths do ESP-IDF v5.5.2
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.5.2
set IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.5_py3.11_env
set IDF_TOOLS_PATH=C:\Espressif

REM Adiciona ao PATH
set PATH=%IDF_PATH%;%IDF_PATH%\tools;%IDF_PYTHON_ENV_PATH%;%IDF_PYTHON_ENV_PATH%\Scripts;%IDF_TOOLS_PATH%\tools\xtensa-esp-elf\esp-14.2.0_20251107\xtensa-esp-elf\bin;%IDF_TOOLS_PATH%\tools\esp32ulp-elf\2.38_20240113;%PATH%

echo.
echo [INFO] ESP-IDF: %IDF_PATH%
echo [INFO] Python: %IDF_PYTHON_ENV_PATH%
echo.

REM Vai para o projeto
cd /d D:\git\Bastao\Bastao-ESP\esp32_firmware

REM Verifica se o ambiente do ESP-IDF mudou em relação ao build anterior
set ENV_CHANGED=0
set LAST_IDF_PATH=
if exist build\idf_path.txt (
    set /p LAST_IDF_PATH=<build\idf_path.txt
) else (
    set ENV_CHANGED=1
)

if "%LAST_IDF_PATH%" neq "%IDF_PATH%" (
    echo [WARN] Detetada alternancia no ambiente do ESP-IDF!
    echo Anterior: "%LAST_IDF_PATH%"
    echo Atual   : "%IDF_PATH%"
    set ENV_CHANGED=1
)

REM Limpa se o ambiente mudou
if "%ENV_CHANGED%"=="1" (
    echo [INFO] Limpando cache de build antigo para evitar conflitos de versao...
    if exist build rd /s /q build
    del /f /q sdkconfig sdkconfig.old 2>nul
)

REM Configura target se necessario
if not exist sdkconfig (
    echo [1/4] Configurando target ESP32-S3 do zero...
    python %IDF_PATH%\tools\idf.py set-target esp32s3
) else (
    findstr /C:"CONFIG_IDF_TARGET=\"esp32s3\"" sdkconfig >nul 2>nul
    if errorlevel 1 (
        echo [WARN] Detectado target diferente de ESP32-S3 no sdkconfig!
        echo Limpando a pasta de build antiga para evitar conflitos de cache...
        if exist build rd /s /q build
        del /f /q sdkconfig sdkconfig.old 2>nul
        echo [1/4] Reconfigurando target para ESP32-S3...
        python %IDF_PATH%\tools\idf.py set-target esp32s3
    ) else (
        echo [INFO] Target ja configurado para ESP32-S3
    )
)

REM Salva o IDF_PATH ativo para futuras comparacoes
if not exist build mkdir build
echo %IDF_PATH%>build\idf_path.txt

REM Compila o projeto
echo.
echo [2/4] Compilando projeto...
python %IDF_PATH%\tools\idf.py build
if errorlevel 1 (
    echo ERRO: Falha na compilacao
    pause
    exit /b 1
)

echo.
echo [3/4] Compilacao concluida!
echo.
echo Para fazer flash: python %IDF_PATH%\tools\idf.py -p COM7 flash
echo Para monitorar:   python %IDF_PATH%\tools\idf.py -p COM7 monitor
echo.

pause