@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
goto :run
:usage
echo usage:
echo   testbuild_Android.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

set _OLD_DIR=%cd%
cd /d "%PROJ_PATH%"
call gradlew.bat
set _RESULT=%errorlevel%
cd /d "%_OLD_DIR%"

exit /b %_RESULT%

