@echo off
setlocal

set WORK_DIR=%~dp0
set ABS_PATH=%~2%
set REF_PATH=%~3%

if not defined ABS_PATH goto :usage
if not defined REF_PATH goto :usage
goto :run
:usage
echo usage:
echo   path_abs2rel.bat RESULT ABS_PATH REF_PATH
exit /b 1
:run

for %%i in (%REF_PATH%) do set REF_PATH_TMP=%%~fi
for %%i in (%ABS_PATH%) do set ABS_PATH_TMP=%%~fi

if not exist "%REF_PATH_TMP%" (
    RESULT=%ABS_PATH_TMP%
    goto :return_result
)
if not exist "%ABS_PATH_TMP%" (
    RESULT=%ABS_PATH_TMP%
    goto :return_result
)

set SAME_PATH=%REF_PATH_TMP%
set SAME_PATH_MATCH=0
:same_path_loop
call set "MATCH_TMP=%%ABS_PATH_TMP:%SAME_PATH%=%%"
if not "%MATCH_TMP%" == "%ABS_PATH_TMP%" (
    set SAME_PATH_MATCH=1
    goto :same_path_loop_end
)
set SAME_PATH_OLD=%SAME_PATH%
for %%a in (%SAME_PATH%\..) do set SAME_PATH=%%~fa
if "%SAME_PATH%" == "%SAME_PATH_OLD%" goto :same_path_loop_end
goto :same_path_loop
:same_path_loop_end

if not %SAME_PATH_MATCH% == 1 (
    RESULT=%ABS_PATH_TMP%
    goto :return_result
)

set RESULT=

call set "REF_PATH_TMP=%%REF_PATH_TMP:%SAME_PATH%=%%"
:abs_parent_loop
if "x%REF_PATH_TMP%" == "x" (
    set RESULT=.
    goto :abs_parent_loop_end
)
set REF_PATH_TMP_OLD=%REF_PATH_TMP%
for %%a in (%REF_PATH_TMP%\..) do set REF_PATH_TMP=%%~fa
if "%REF_PATH_TMP%" == "%REF_PATH_TMP_OLD%" goto :abs_parent_loop_end
if "x%RESULT%" == "x" (
    set RESULT=..
) else (
    set RESULT=%RESULT%\..
)
goto :abs_parent_loop
:abs_parent_loop_end

call set "RESULT_TMP=%%ABS_PATH_TMP:%SAME_PATH%=%%"
set RESULT=%RESULT%%RESULT_TMP%

:return_result
endlocal & set "%1=%RESULT%"

