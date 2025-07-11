#!/bin/bash

# 设置脚本在遇到错误时继续执行（类似bat文件的行为）
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "[信息] 当前工作目录: $(pwd)"

# 先调用build.sh进行编译
echo "[信息] 开始编译项目..."
./build.sh

# 检查编译是否成功（这里我们检查是否能找到可执行文件）
if [ ! -f "build/Debug/zengine" ] && [ ! -f "build/zengine" ]; then
    echo "[错误] 编译失败，无法运行程序"
    exit 1
fi

echo "[信息] 编译成功，开始运行程序..."
./run.sh
