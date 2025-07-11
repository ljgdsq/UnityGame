#!/bin/bash

# 设置脚本在遇到错误时继续执行（类似bat文件的行为）
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "[信息] 当前工作目录: $(pwd)"

echo "[信息] 开始运行程序..."

# 检查可执行文件
if [ -f "build/Debug/zengine" ]; then
    echo "[信息] 运行: build/Debug/zengine"
    ./build/Debug/zengine
elif [ -f "build/zengine" ]; then
    echo "[信息] 运行: build/zengine"
    ./build/zengine
else
    echo "[错误] 找不到可执行文件"
fi

echo "[信息] 程序运行完毕"
