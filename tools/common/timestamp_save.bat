@echo off
setlocal

set WORK_DIR=%~dp0
set DIR_TO_CHECK=%~1%
set CHECK_INTERVAL=%~2%
set TIMESTAMP_FILE_NAME=%~3%

if not defined DIR_TO_CHECK goto :usage
if not defined CHECK_INTERVAL goto :usage
goto :run
:usage
echo usage:
echo   timestamp_save.bat DIR_TO_CHECK CHECK_INTERVAL [TIMESTAMP_FILE_NAME]
exit /b 1
:run

if not defined TIMESTAMP_FILE_NAME set TIMESTAMP_FILE_NAME=_zf_timestamp_

call :GetUnixTime _CUR_TIME
mkdir "%DIR_TO_CHECK%" >nul 2>&1
echo %_CUR_TIME% > "%DIR_TO_CHECK%\%TIMESTAMP_FILE_NAME%"
>"%DIR_TO_CHECK%\%TIMESTAMP_FILE_NAME%" set /p=%_CUR_TIME%<nul

goto :EOF
:GetUnixTime
setlocal enableextensions
for /f %%x in ('wmic path win32_utctime get /format:list ^<nul ^| findstr "="') do set %%x
set /a z=(14-100%Month%%%100)/12, y=10000%Year%%%10000-z
set /a ut=y*365+y/4-y/100+y/400+(153*(100%Month%%%100+12*z-3)+2)/5+Day-719469
set /a ut=ut*86400+100%Hour%%%100*3600+100%Minute%%%100*60+100%Second%%%100
del /f/s/q TempWmicBatchFile.bat >nul 2>&1
endlocal & set "%1=%ut%" & goto :EOF

