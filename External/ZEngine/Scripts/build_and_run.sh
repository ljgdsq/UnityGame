#!/bin/bash

# 设置脚本在遇到错误时继续执行（类似bat文件的行为）
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "[信息] 当前工作目录: $(pwd)"

# 先调用build.sh进行编译
echo "[信息] 开始编译项目..."

#!/bin/bash

# 设置脚本在遇到错误时继续执行
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "[信息] 当前工作目录: $(pwd)"

echo "[信息] 开始编译项目..."
bash build.sh

if [ $? -ne 0 ]; then
    echo "[错误] 编译失败，无法运行程序"
    exit 1
fi

echo "[信息] 编译成功，开始运行..."
bash run.sh

echo "[信息] Build & Run流程完成"
