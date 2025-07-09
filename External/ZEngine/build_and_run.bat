@echo off
:: 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul
@REM echo [信息] 设置控制台编码为UTF-8

:: 确保当前目录是脚本所在目录
cd /d "%~dp0"
echo [信息] 当前工作目录: %cd%

:: 先调用build.bat进行编译
echo [信息] 开始编译项目...
call build.bat

:: 检查编译是否成功
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 编译失败，无法运行程序
    pause
    exit /b %ERRORLEVEL%
)

call run.bat

:: 暂停，以便查看输出
pause
