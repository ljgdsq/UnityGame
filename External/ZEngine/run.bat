@echo off
:: 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul
@REM echo [信息] 设置控制台编码为UTF-8

:: 确保当前目录是脚本所在目录
cd /d "%~dp0"
echo [信息] 当前工作目录: %cd%

echo [信息] 开始运行程序...

:: 检查Debug目录下的可执行文件
if exist build\Debug\zengine.exe (
    echo [信息] 运行: build\Debug\zengine.exe
    build\Debug\zengine.exe
) else if exist build\zengine.exe (
    echo [信息] 运行: build\zengine.exe
    build\zengine.exe
) else (
    echo [错误] 找不到可执行文件
)

echo [信息] 程序运行完毕
