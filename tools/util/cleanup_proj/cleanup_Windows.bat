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
echo   cleanup_Windows.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem Windows
del /f/s/q "%PROJ_ROOT%\*.ncb" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\*.suo" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\*.vcproj.*.user" >nul 2>&1

exit /b 0

