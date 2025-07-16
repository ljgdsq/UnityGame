
@echo off
REM ===============================
REM ZEngine Build & Run Script (Scripts/build_and_run.bat)
REM ===============================

REM 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul

REM 记录脚本起始目录
set ROOT_DIR=%~dp0..
cd /d "%ROOT_DIR%\Scripts"
echo [信息] 当前工作目录: %cd%

REM 编译项目
echo [信息] 开始编译项目...
call build.bat

REM 检查编译是否成功
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 编译失败，无法运行程序
    exit /b %ERRORLEVEL%
)

REM 运行项目
call run.bat

echo [信息] Build and Run流程完成
