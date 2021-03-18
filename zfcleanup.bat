@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\ZFFramework
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set CLEAN_WHAT=%~1%

set _CLEANUP_build=0
if not defined CLEAN_WHAT (
    set _CLEANUP_build=1
) else (
    if "%CLEAN_WHAT%" == "build" set _CLEANUP_build=1
)
if %_CLEANUP_build% == 1 (
    call "%ZF_TOOLS_PATH%\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_cleanup*.zfsh"
    call "%ZF_TOOLS_PATH%\common\zfsh_recursive.bat" "%WORK_DIR%\..\ZFModule" "zfautoscript_cleanup*.zfsh"
)

set _CLEANUP_zf3rd=0
if not defined CLEAN_WHAT (
    set _CLEANUP_zf3rd=1
) else (
    if "%CLEAN_WHAT%" == "zf3rd" set _CLEANUP_zf3rd=1
)
if %_CLEANUP_zf3rd% == 1 (
    call "%ZF_TOOLS_PATH%\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_zf3rd_cleanup*.zfsh"
    call "%ZF_TOOLS_PATH%\common\zfsh_recursive.bat" "%WORK_DIR%\..\ZFModule" "zfautoscript_zf3rd_cleanup*.zfsh"
)

rmdir /s/q "%WORK_DIR%\_release" >nul 2>&1
rmdir /s/q "%WORK_DIR%\_tmp" >nul 2>&1

