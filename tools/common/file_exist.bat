@echo off
setlocal

set WORK_DIR=%~dp0
set PATH_TO_CHECK=%~1%
set FILE_PATTERN=%~2%

if not defined PATH_TO_CHECK goto :usage
if not defined FILE_PATTERN goto :usage
goto :run
:usage
echo usage:
echo   file_exist.bat PATH_TO_CHECK FILE_PATTERN
echo return by "errorlevel":
echo   0: exist
echo   1: not exist
exit /b 1
:run

set RESULT=0
dir /s /b "%PATH_TO_CHECK%\%FILE_PATTERN%" >nul 2>&1 && set RESULT=1
if %RESULT%==1 (
    exit /b 0
) else (
    exit /b 1
)

