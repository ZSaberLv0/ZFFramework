@echo off
setlocal

set WORK_DIR=%~dp0
set SCRIPT_FILE=%~1%

if not defined SCRIPT_FILE goto :usage
goto :run
:usage
echo usage:
echo   zfsh_if_exist.bat SCRIPT_FILE [...]
exit /b 1
:run

if not exist "%SCRIPT_FILE%" (
    exit /b 0
)

for /f "tokens=2,* delims= " %%a in ("%*") do set ALL_VAR=%%a
call "%WORK_DIR%\zfsh.bat" "%SCRIPT_FILE%" %ALL_VAR%

