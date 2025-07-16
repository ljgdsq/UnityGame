
@echo off
REM ===============================
REM ZEngine Run Script (Scripts/run.bat)
REM ===============================

REM 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul

REM 记录脚本起始目录
set ROOT_DIR=%~dp0..
cd /d "%ROOT_DIR%"
echo [信息] 当前工作目录: %cd%

REM 构建输出目录
set BUILD_DIR=%ROOT_DIR%\build
set DEBUG_EXE=%BUILD_DIR%\Debug\zengine.exe
set RELEASE_EXE=%BUILD_DIR%\Release\zengine.exe
set FLAT_EXE=%BUILD_DIR%\zengine.exe

echo [信息] 开始运行程序...

REM 优先运行Debug版本
if exist "%DEBUG_EXE%" (
    echo [信息] 运行: %DEBUG_EXE%
    "%DEBUG_EXE%"
) else if exist "%RELEASE_EXE%" (
    echo [信息] 运行: %RELEASE_EXE%
    "%RELEASE_EXE%"
) else if exist "%FLAT_EXE%" (
    echo [信息] 运行: %FLAT_EXE%
    "%FLAT_EXE%"
) else (
    echo [错误] 找不到可执行文件 (Debug/Release/zengine.exe)
)

echo [信息] 程序运行完毕
