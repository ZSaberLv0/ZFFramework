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
echo   timestamp_check.bat DIR_TO_CHECK CHECK_INTERVAL [TIMESTAMP_FILE_NAME]
echo return by "errorlevel":
echo   0: not expired
echo   1: expired
exit /b 1
:run

if not defined TIMESTAMP_FILE_NAME set TIMESTAMP_FILE_NAME=_zf_timestamp_

call :GetUnixTime _CUR_TIME
if exist "%DIR_TO_CHECK%\%TIMESTAMP_FILE_NAME%" (
    set /p _OLD_TIME=<"%DIR_TO_CHECK%\%TIMESTAMP_FILE_NAME%"
) else (
    set _OLD_TIME=0
)
set /a _EXPIRE_TIME=%_OLD_TIME% + %CHECK_INTERVAL%
if %_CUR_TIME% GEQ %_EXPIRE_TIME% (
    exit /b 1
)


goto :EOF
:GetUnixTime
setlocal enableextensions
for /f %%x in ('wmic path win32_utctime get /format:list ^<nul ^| findstr "="') do set %%x
set /a z=(14-100%Month%%%100)/12, y=10000%Year%%%10000-z
set /a ut=y*365+y/4-y/100+y/400+(153*(100%Month%%%100+12*z-3)+2)/5+Day-719469
set /a ut=ut*86400+100%Hour%%%100*3600+100%Minute%%%100*60+100%Second%%%100
del /f/s/q TempWmicBatchFile.bat >nul 2>&1
endlocal & set "%1=%ut%" & goto :EOF

