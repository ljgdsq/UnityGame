#!/bin/bash

# 设置脚本在遇到错误时继续执行（类似bat文件的行为）
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "[信息] 当前工作目录: $(pwd)"

# 如果不存在build目录，则创建
if [ ! -d "build" ]; then
    echo "[信息] 创建build目录..."
    mkdir -p build
fi

# 进入build目录
cd build
echo "[信息] 进入build目录: $(pwd)"

# 运行CMake配置
echo "[信息] 运行CMake配置..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# 检查CMake配置是否成功
if [ $? -eq 0 ]; then
    echo "[信息] CMake配置成功，开始构建项目..."
    cmake --build . --config Debug
    
    # 检查构建结果
    if [ $? -eq 0 ]; then
        echo "[信息] 项目构建成功！"
    else
        echo "[错误] 项目构建失败，错误代码: $?"
    fi
else
    echo "[错误] CMake配置失败，错误代码: $?"
fi

# 返回到原始目录
cd ..
echo "[信息] 返回原始目录: $(pwd)"

echo "[信息] 构建过程完成"
