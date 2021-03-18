@echo off
setlocal

rem to make doc, copy these files from doxygen binary to:
rem   master\ZF_docs\Doxygen\doxygen.exe
rem   master\ZF_docs\Doxygen\libclang.dll

set WORK_DIR=%~dp0

set OUTPUT_PATH=%WORK_DIR%\..\..\_tmp\DoxygenDoc

rd /s/q %OUTPUT_PATH% >nul 2>&1
mkdir %OUTPUT_PATH% >nul 2>&1

set _OLD_DIR=%cd%
cd /d %WORK_DIR%\Doxygen
.\doxygen.exe
cd /d "%_OLD_DIR%"

set _OLD_DIR=%cd%
cd /d %OUTPUT_PATH%\html
%WORK_DIR%\Doxygen\hhc.exe index.hhp
cd /d "%_OLD_DIR%"

move %OUTPUT_PATH%\html\index.chm %OUTPUT_PATH%\ZFFramework_doc.chm
del /q %OUTPUT_PATH%\html\index.hhc >nul 2>&1
del /q %OUTPUT_PATH%\html\index.hhk >nul 2>&1
del /q %OUTPUT_PATH%\html\index.hhp >nul 2>&1

