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
echo   cleanup_Android.bat PROJ_ROOT PROJ_NAME
exit /b 1
:run

rem ============================================================
rem Android
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\.gradle" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\.idea" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\build" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\%PROJ_NAME%\local.properties" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\%PROJ_NAME%\*.iml" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\zflib\.externalNativeBuild" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\zflib\build" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\%PROJ_NAME%\zflib\*.iml" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\zfapp\.externalNativeBuild" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\zfapp\build" >nul 2>&1
del /f/s/q "%PROJ_ROOT%\%PROJ_NAME%\zfapp\*.iml" >nul 2>&1
rmdir /s/q "%PROJ_ROOT%\%PROJ_NAME%\zfapp\src\main\assets\zfres" >nul 2>&1

exit /b 0

