@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set PROJ_GIT=%~1%
set GIT_BRANCH=%~2%
set DST_PATH=%~3%
for /f "tokens=4,* delims= " %%a in ("%*") do set CLONE_OPTION=%%a

if not defined PROJ_GIT goto :usage
if not defined GIT_BRANCH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   git_check.bat PROJ_GIT GIT_BRANCH DST_PATH [options]
exit /b 1
:run
if not defined CLONE_OPTION set CLONE_OPTION=--depth=1

set _OLD_DIR=%cd%
rem 60*60*24*7, one week
set _TIMEOUT=604800
set _GIT_VALID=0

if exist "%DST_PATH%\.git" (
    set _GIT_PATH_DESIRED=%DST_PATH%\.git
    cd /d "%_GIT_PATH_DESIRED%" >nul 2>&1
    set _GIT_PATH_DESIRED=%CD%
    cd /d "%_OLD_DIR%"

    cd /d "%DST_PATH%" >nul 2>&1
    for /f "tokens=*" %%i in ('git rev-parse --show-toplevel 2^>nul') do set _GIT_PATH_EXIST=%%i\.git
    cd /d "%_OLD_DIR%"
    cd /d "%_GIT_PATH_EXIST%" >nul 2>&1
    set _GIT_PATH_EXIST=%CD%
    cd /d "%_OLD_DIR%"

    if "%_GIT_PATH_DESIRED%" == "%_GIT_PATH_EXIST%" (
        set _GIT_VALID=1
    )
)

if "%_GIT_VALID%" == "1" (
    call "%WORK_DIR%\timestamp_check.bat" "%DST_PATH%\.git" %_TIMEOUT%

    if not "!errorlevel!" == "0" (
        cd /d "%DST_PATH%"
        git stash
        git checkout .
        git reset --hard
        git fetch --all && git pull
        set _SUCCESS=!errorlevel!
        git checkout %GIT_BRANCH%
        git stash pop
        cd /d "%_OLD_DIR%"

        if "!_SUCCESS!" == "0" (
            call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" %_TIMEOUT%
        )
    )
) else (
    rmdir /s/q "%DST_PATH%" >nul 2>&1
    mkdir "%DST_PATH%" >nul 2>&1
    git clone -b %GIT_BRANCH% %CLONE_OPTION% "%PROJ_GIT%" "%DST_PATH%"

    if "!errorlevel!" == "0" (
        call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git" %_TIMEOUT%
    )
)

exit /b 0

