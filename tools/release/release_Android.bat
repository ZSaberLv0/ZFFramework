@echo off
setlocal

set WORK_DIR=%~dp0
set CHECK_ONLY=%~1%
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools

if not defined CHECK_ONLY goto :run
if not exist "%ZF_ROOT_PATH%\_release\Android\all\aar" goto :run
exit /b 0
:run

call "%ZF_TOOLS_PATH%\common\zfsh.bat" "%WORK_DIR%\release.zfsh" Android

