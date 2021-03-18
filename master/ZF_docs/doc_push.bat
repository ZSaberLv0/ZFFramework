@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\..
set ZF_TOOLS_PATH=%ZF_ROOT_PATH%\tools
set OUTPUT_PATH=%WORK_DIR%\Doxygen\..\..\..\_tmp\DoxygenDoc

set PASSWORD=%~1%
if not defined PASSWORD (
    set /p PASSWORD="enter password: "
    if not defined PASSWORD (
        echo canceled
        exit /b 1
    )
)


if not exist "%OUTPUT_PATH%\ZFFramework_doc.chm" (
    echo doc not exist
    exit /b 1
)

set DOC_REPO_PATH=%OUTPUT_PATH%\..\DoxygenDoc_repo

call "%ZF_TOOLS_PATH%\common\git_check.bat" "https://github.com/ZFFramework/zfframework.github.com" master "%DOC_REPO_PATH%"
xcopy /s/e/y/r/h "%WORK_DIR%\doc_repo" "%DOC_REPO_PATH%"
rmdir /s/q "%DOC_REPO_PATH%\doc" >nul 2>&1
mkdir "%DOC_REPO_PATH%\doc" >nul 2>&1
xcopy /s/e/y/r/h "%OUTPUT_PATH%\html" "%DOC_REPO_PATH%\doc"

set _OLD_DIR=%cd%
cd /d "%DOC_REPO_PATH%"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git add -A
git commit -a -m "update doc"
git push "https://ZSaberLv0:%PASSWORD%@github.com/ZFFramework/zfframework.github.com"
git fetch origin --prune
cd /d "%_OLD_DIR%"

