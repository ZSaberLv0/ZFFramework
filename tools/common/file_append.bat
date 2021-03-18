@echo off
setlocal

set WORK_DIR=%~dp0
set DST_PATH=%~1%
set SRC_PATH=%~2%

if not defined DST_PATH goto :usage
if not defined SRC_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_append.bat DST_PATH SRC_PATH
exit /b 1
:run

for %%a in (%DST_PATH%\..) do set _DST_PARENT=%%~fa
mkdir "%_DST_PARENT%" >nul 2>&1

>nul 2>&1 (
    more "%SRC_PATH%" >> "%DST_PATH%"
)

