@echo off
setlocal

set WORK_DIR=%~dp0
set ZFSH_PATH=%~1%

if not defined ZFSH_PATH goto :usage
goto :run
:usage
echo usage:
echo   zfsh.bat ZFSH_PATH [...]
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..
for %%a in (%ZFSH_PATH%\..) do set ZFSH_WORK_DIR=%%~fa
for %%a in (%ZFSH_PATH%) do set TMP_PATH=%%~nxa
set TMP_PATH=%ZF_ROOT_PATH%\_tmp\zfsh_tmp_%TMP_PATH%.bat

del /f/s/q "%TMP_PATH%" >nul 2>&1
for %%a in (%TMP_PATH%\..) do set TMP_PATH_PARENT=%%~fa
mkdir %TMP_PATH_PARENT% >nul 2>&1

setlocal enabledelayedexpansion

echo @echo off>>"%TMP_PATH%"
echo setlocal>>"%TMP_PATH%"
echo set _ZF_P0=%%~0%%>>"%TMP_PATH%"
echo set _ZF_P1=%%~1%%>>"%TMP_PATH%"
echo set _ZF_P2=%%~2%%>>"%TMP_PATH%"
echo set _ZF_P3=%%~3%%>>"%TMP_PATH%"
echo set _ZF_P4=%%~4%%>>"%TMP_PATH%"
echo set _ZF_P5=%%~5%%>>"%TMP_PATH%"
echo set _ZF_P6=%%~6%%>>"%TMP_PATH%"
echo set _ZF_P7=%%~7%%>>"%TMP_PATH%"
echo set _ZF_P8=%%~8%%>>"%TMP_PATH%"

for /f "tokens=*" %%a in (%ZFSH_PATH%) do (
    set line=%%a

    set line=!line:^<ZF_COMMENT^>=rem!
    set line=!line:^<ZF_SH^>=call!
    set line=!line:^<ZF_SH_EXT^>=bat!

    set line=!line:^<ZF_ROOT_PATH^>=%ZF_ROOT_PATH%!
    set line=!line:^<ZF_WORK_DIR^>=%ZFSH_WORK_DIR%!

    set line=!line:^<ZF_P0^>=%%_ZF_P0%%!
    set line=!line:^<ZF_P1^>=%%_ZF_P1%%!
    set line=!line:^<ZF_P2^>=%%_ZF_P2%%!
    set line=!line:^<ZF_P3^>=%%_ZF_P3%%!
    set line=!line:^<ZF_P4^>=%%_ZF_P4%%!
    set line=!line:^<ZF_P5^>=%%_ZF_P5%%!
    set line=!line:^<ZF_P6^>=%%_ZF_P6%%!
    set line=!line:^<ZF_P7^>=%%_ZF_P7%%!
    set line=!line:^<ZF_P8^>=%%_ZF_P8%%!

    set line=!line:^<ZF_SET^>=set!
    set line=!line:^<ZF_PL^>=%%!
    set line=!line:^<ZF_PR^>=%%!
    set line=!line:^<ZF_EXIT_SUCCESS^>=exit /b 0!
    set line=!line:^<ZF_EXIT_FAILED^>=exit /b 1!
    set line=!line:^<ZF_IF_SUCCESS_BEGIN^>=if "%%errorlevel%%" == "0" (!
    set line=!line:^<ZF_IF_SUCCESS_END^>=^)!
    set line=!line:^<ZF_IF_FAILED_BEGIN^>=if not "%%errorlevel%%" == "0" (!
    set line=!line:^<ZF_IF_FAILED_END^>=^)!

    echo !line!>>"%TMP_PATH%"
)

for /f "tokens=2,* delims= " %%a in ("%*") do set ALL_BUT_FIRST=%%a
call "%TMP_PATH%" %ALL_BUT_FIRST%
set RESULT=%errorlevel%
del /f/s/q "%TMP_PATH%" >nul 2>&1
exit /b %RESULT%

