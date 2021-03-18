@echo off
setlocal

set WORK_DIR=%~dp0
set TARGET_PATH=%~1%

if not defined TARGET_PATH goto :usage
goto :run
:usage
echo usage:
echo   mkdir.bat TARGET_PATH
exit /b 1
:run

mkdir "%TARGET_PATH%" >nul 2>&1

