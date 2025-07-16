
@echo off
REM ===============================
REM ZEngine Build Script (Scripts/build.bat)
REM ===============================

REM 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul

REM 记录脚本起始目录
set ROOT_DIR=%~dp0..
cd /d "%ROOT_DIR%"
echo [信息] 当前工作目录: %cd%

REM 构建输出目录
set BUILD_DIR=%ROOT_DIR%\build

REM 如果不存在build目录，则创建
if not exist "%BUILD_DIR%" (
    echo [信息] 创建build目录...
    mkdir "%BUILD_DIR%"
)

REM 进入build目录
cd /d "%BUILD_DIR%"
echo [信息] 进入build目录: %cd%

REM 运行CMake配置
echo [信息] 运行CMake配置...
cmake .. -DCMAKE_BUILD_TYPE=Debug

REM 如果CMake配置成功，则构建项目
if %ERRORLEVEL% EQU 0 (
    echo [信息] CMake配置成功，开始构建项目...
    cmake --build . --config Debug
    if %ERRORLEVEL% EQU 0 (
        echo [信息] 项目构建成功！
    ) else (
        echo [错误] 项目构建失败，错误代码: %ERRORLEVEL%
    )
) else (
    echo [错误] CMake配置失败，错误代码: %ERRORLEVEL%
)

REM 返回到脚本目录
cd /d "%ROOT_DIR%\Scripts"
echo [信息] 返回Scripts目录: %cd%

echo [信息] 构建过程完成

REM 如需调试可取消注释
REM pause
