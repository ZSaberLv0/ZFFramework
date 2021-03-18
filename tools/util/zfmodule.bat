@echo off
setlocal

set WORK_DIR=%~dp0
set MODULE_REPO=%~1%
set MODULE_NAME=%~2%
set MODULE_BRANCH=%~2%

if not defined MODULE_REPO goto :usage
goto :run
:usage
echo usage:
echo   zfmodule.bat MODULE_REPO [MODULE_NAME MODULE_BRANCH]
exit /b 1
:run

if not defined MODULE_NAME for %%a in (%MODULE_REPO%) do set MODULE_NAME=%%~nxa
if not defined MODULE_BRANCH set MODULE_BRANCH=master

set ZF_ROOT_PATH=%WORK_DIR%\..\..

if "%MODULE_BRANCH%" neq "LOCAL" (
    call "%ZF_ROOT_PATH%\tools\common\git_check.bat" %MODULE_REPO% %MODULE_BRANCH% "%ZF_ROOT_PATH%\..\ZFModule\ZF\%MODULE_NAME%"
)

