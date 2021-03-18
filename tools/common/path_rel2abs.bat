@echo off
setlocal

set WORK_DIR=%~dp0
set REL_PATH=%~2%

if not defined REL_PATH goto :usage
goto :run
:usage
echo usage:
echo   path_rel2abs.bat RESULT REL_PATH
exit /b 1
:run

pushd %REL_PATH%
set ABS_PATH=%CD%
popd

endlocal & set "%1=%ABS_PATH%"

