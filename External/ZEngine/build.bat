@echo off
:: 设置控制台代码页为UTF-8以支持中文显示
chcp 65001 > nul
echo [信息] 设置控制台编码为UTF-8

:: 确保当前目录是脚本所在目录
cd /d "%~dp0"
echo [信息] 当前工作目录: %cd%

:: 如果不存在build目录，则创建
if not exist build (
    echo [信息] 创建build目录...
    mkdir build
)

:: 进入build目录
cd build
echo [信息] 进入build目录: %cd%

:: 运行CMake配置
echo [信息] 运行CMake配置...
cmake .. -DCMAKE_BUILD_TYPE=Debug

:: 如果CMake配置成功，则构建项目
if %ERRORLEVEL% EQU 0 (
    echo [信息] CMake配置成功，开始构建项目...
    cmake --build . --config Debug
    
    :: 检查构建结果
    if %ERRORLEVEL% EQU 0 (
        echo [信息] 项目构建成功！
    ) else (
        echo [错误] 项目构建失败，错误代码: %ERRORLEVEL%
    )
) else (
    echo [错误] CMake配置失败，错误代码: %ERRORLEVEL%
)

:: 返回到原始目录
cd ..
echo [信息] 返回原始目录: %cd%

echo [信息] 构建过程完成

:: 暂停，以便查看输出
pause
