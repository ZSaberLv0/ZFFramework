@echo off
setlocal

set WORK_DIR=%~dp0
set SRC_PATH=%~1%
set DST_PATH=%~2%

if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   mv.bat SRC_PATH DST_PATH
exit /b 1
:run

move /y "%SRC_PATH%" "%DST_PATH%" >nul 2>&1

