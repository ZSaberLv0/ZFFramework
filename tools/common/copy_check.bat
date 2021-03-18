@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_PATH=%~1%
set DST_PATH=%~2%

if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   copy_check.bat SRC_PATH DST_PATH
echo "note: path must not end with \"
exit /b 1
:run

if not exist "%SRC_PATH%\*" (
    call :CopyByMd5Check "%SRC_PATH%" "%DST_PATH%"
    exit /b 0
)

for /f %%f in ("%SRC_PATH%") do set _SRC_ROOT=%%~ff
for /f "tokens=*" %%f in ('dir /a-d/s/b "%SRC_PATH%" 2^>nul') do (
    set _SRC_FILE_PATH=%%~ff
    set _SRC_RELATIVE=!_SRC_FILE_PATH:%_SRC_ROOT%\=!
    call :CopyByMd5Check "!_SRC_FILE_PATH!" "%DST_PATH%\!_SRC_RELATIVE!"
)

exit /b 0

:CopyByMd5Check
for %%a in (%~f2\..) do set DST_PARENT=%%~fa
mkdir "%DST_PARENT%" >nul 2>&1
if not exist "%~f2" (
    copy /y "%~f1" "%~f2" >nul 2>&1
    endlocal & goto :EOF
)
call :GetMd5 _SRC_MD5 "%~f1"
call :GetMd5 _DST_MD5 "%~f2"
if "x-%_SRC_MD5%" neq "x-%_DST_MD5%" (
    copy /y "%~f1" "%~f2" >nul 2>&1
)
endlocal & goto :EOF

:GetMd5
set "md5="
if not exist "%~2" (
    set "%1=%md5%"
    goto :EOF
)
for /f "skip=1 tokens=* delims=" %%# in ('certutil -hashfile "%~f2" MD5') do (
    if not defined md5 (
        for %%Z in (%%#) do set "md5=!md5!%%Z"
    )
)
endlocal & set "%1=%md5%" & goto :EOF

