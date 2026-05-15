@echo off
cd /d "%~dp0"

if "%~1"=="" (
    echo 請把 .ps1 檔案拖曳到這個 build_exe.cmd 上面。
    echo.
    pause
    exit /b
)

set "INPUT=%~1"
set "OUT=%~dpn1.exe"
set "ICON=%~dp0setup.ico"

echo.
echo 來源檔案：
echo %INPUT%
echo.
echo 輸出檔案：
echo %OUT%
echo.

if not exist "%INPUT%" (
    echo 找不到來源檔案。
    pause
    exit /b
)

if not exist "%ICON%" (
    echo 找不到 setup.ico，請確認 setup.ico 跟 build_exe.cmd 放在同一個資料夾。
    pause
    exit /b
)

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
"Invoke-PS2EXE ^
-inputFile '%INPUT%' ^
-outputFile '%OUT%' ^
-noConsole ^
-requireAdmin ^
-noOutput ^
-noError ^
-iconFile '%ICON%' ^
-title '印表機安裝工具' ^
-description '印表機驅動與設定安裝工具' ^
-product '印表機安裝工具' ^
-version '1.0.0.0' ^
-copyright 'Copyright (c) Chuan Jia'"

echo.
if exist "%OUT%" (
    echo 完成，已產生：
    echo %OUT%
) else (
    echo 轉換失敗，沒有產生 EXE。
)

echo.
pause
