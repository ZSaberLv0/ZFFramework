@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set SRC_PATH=%~2%
set DST_PATH=%~3%

if not defined PROJ_NAME goto :usage
if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   copy_lib.bat PROJ_NAME SRC_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>&1
copy /y "%SRC_PATH%\\*.a" "%DST_PATH%" >nul 2>&1
copy /y "%SRC_PATH%\\*.dll" "%DST_PATH%" >nul 2>&1

exit /b 0

