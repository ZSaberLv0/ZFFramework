@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_ROOT=%~1%
set PROJ_NAME=%~2%

if not defined PROJ_ROOT goto :usage
if not defined PROJ_NAME goto :usage
goto :run
:usage
echo usage:
echo   cleanup_Qt.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem Qt
for /f "tokens=*" %%i in ('dir /a:d /s /b "%PROJ_ROOT%\build-*" 2^>nul') do (
    rmdir /s/q %%i >nul 2>&1
)

del /f/s/q "%PROJ_ROOT%\*.pro.user" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\*.pro.user.*" >nul 2>&1

exit /b 0

