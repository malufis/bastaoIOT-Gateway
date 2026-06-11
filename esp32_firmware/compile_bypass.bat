@echo off
REM Script para compilar o Bastao-ESP sem dependencias quebradas usando ESP-IDF v5.5.2

echo ================================================
echo   Compilando Bastao-ESP (Bypass Dependencies v5.5.2)
echo ================================================

REM Define variaveis do ESP-IDF v5.5.2
set IDF_PATH=C:\Espressif\frameworks\esp-idf-v5.5.2
set IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.5_py3.11_env
set PATH=%IDF_PATH%;%IDF_PYTHON_ENV_PATH%\Scripts;%PATH%

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
    echo [1/3] Limpando e configurando ESP32-S3 do zero...
    if exist build rd /s /q build
    idf.py set-target esp32s3
) else (
    findstr /C:"CONFIG_IDF_TARGET=\"esp32s3\"" sdkconfig >nul 2>nul
    if errorlevel 1 (
        echo [WARN] Target diferente de ESP32-S3 detectado no sdkconfig!
        echo Limpando a pasta de build antiga e sdkconfig...
        if exist build rd /s /q build
        del /f /q sdkconfig sdkconfig.old 2>nul
        echo [1/3] Configurando target para ESP32-S3...
        idf.py set-target esp32s3
    ) else (
        echo [INFO] Target ja configurado para ESP32-S3. Nao sera necessaria limpeza completa.
    )
)

REM Salva o IDF_PATH ativo para futuras comparacoes
if not exist build mkdir build
echo %IDF_PATH%>build\idf_path.txt

echo.
echo [2/3] Compilando...
idf.py build --no-verify-deps

echo.
echo Done!
pause