@echo off
setlocal

set WORK_DIR=%~dp0

set /p PASSWORD="enter password: "

call "%WORK_DIR%\doc_make.bat"
call "%WORK_DIR%\doc_push.bat" %PASSWORD%

