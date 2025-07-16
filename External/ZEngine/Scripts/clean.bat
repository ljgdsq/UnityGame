@echo off
REM ===============================
REM ZEngine Clean Script (Windows)
REM ===============================

REM 获取脚本所在目录
set SCRIPT_DIR=%~dp0

REM 拼接 build 目录绝对路径
set BUILD_DIR=%SCRIPT_DIR%..\build

REM 删除 build 目录及所有内容
if exist "%BUILD_DIR%" (
    echo [信息] 删除 build 目录...
    rmdir /s /q "%BUILD_DIR%"
    echo [信息] build 目录已删除。
) else (
    echo [信息] 未找到 build 目录，无需清理。
)

REM 可扩展：清理其他临时文件
REM ...

echo [信息] 清理完成。
@echo off
REM ===============================
REM ZEngine Clean Script (Windows)
REM ===============================

REM 删除 build 目录及所有内容
if exist "..\build" (
    echo [信息] 删除 build 目录...
    rmdir /s /q "..\build"
    echo [信息] build 目录已删除。
) else (
    echo [信息] 未找到 build 目录，无需清理。
)

REM 可扩展：清理其他临时文件
REM ...

echo [信息] 清理完成。
