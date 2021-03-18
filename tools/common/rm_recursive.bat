@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_DIR=%~1%
set FILE_NAME=%~2%

if not defined SRC_DIR goto :usage
if not defined FILE_NAME goto :usage
goto :run
:usage
echo usage:
echo   rm_recursive.sh SRC_DIR FILE_NAME
echo set ZF_EXCLUDE to exclude dirs, separated by space, match exact, e.g.
echo   set ZF_EXCLUDE=f0 f1
exit /b 1
:run

rem ============================================================
rem default exclude dirs
set ZF_EXCLUDE_TMP=%ZF_EXCLUDE%
rem set ZF_EXCLUDE_TMP=%ZF_EXCLUDE_TMP% private

rem ============================================================
for /f "tokens=*" %%f in ('dir /a-d/s/b "%SRC_DIR%\%FILE_NAME%" 2^>nul') do (
    del /f/s/q "%%f" >nul 2>&1
    rmdir /s/q "%%f" >nul 2>&1
)

exit /b 0

