@echo off
REM Script para compilar o Bastão-ESP sem dependências quebradas

echo ================================================
echo   Compilando Bastão-ESP (Bypass Dependencies)
echo ================================================

REM Define variáveis
set IDF_PATH=C:\Users\rlmal\esp\v5.4.1\esp-idf
set IDF_PYTHON_ENV_PATH=C:\Espressif\python_env\idf5.4_py3.9_env
set PATH=%IDF_PATH%;%IDF_PYTHON_ENV_PATH%\Scripts;%PATH%

cd /d D:\git\Bastao\Bastão-ESP\esp32_firmware

echo.
echo [1/3] Limpando build anterior...
if exist build rmdir /s /q build

echo.
echo [2/3] Configurando ESP32...
idf.py set-target esp32 --force

echo.
echo [3/3] Compilando...
idf.py build --no-verify-deps

echo.
echo Done!
pause