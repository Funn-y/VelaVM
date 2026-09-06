@echo off
echo auto uploading 'main' branch
echo.

git add .

echo ==============================================================

echo commit:
set /p commit=:

:: 如果提交信息为空，设置默认值
if "%commit%"=="" set commit=auto update

git commit -m "%commit%"

:: 检查 commit 是否成功
if errorlevel 1 (
    echo No changes to commit
    pause
    exit /b 0
)

echo ==============================================================

:: 先拉取再推送，避免冲突
git pull origin main --no-edit

git push -u origin main

echo ==============================================================

pause