@echo off
setlocal

set WORK_DIR=%~dp0
set TARGET_PATH=%~1%

if not defined TARGET_PATH goto :usage
goto :run
:usage
echo usage:
echo   rm.bat TARGET_PATH
exit /b 1
:run

del /f/s/q "%TARGET_PATH%" >nul 2>&1
rmdir /s/q "%TARGET_PATH%" >nul 2>&1
exit /b 0

