#!/bin/bash

# 设置脚本在遇到错误时继续执行（类似bat文件的行为）
set +e

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${SCRIPT_DIR}/.."
cd "$PROJECT_ROOT"

echo "[信息] 当前工作目录: $(pwd)"

echo "[信息] 开始运行程序..."

DEBUG_EXE="build/Debug/zengine"
RELEASE_EXE="build/Release/zengine"
FLAT_EXE="build/zengine"

if [ -f "$DEBUG_EXE" ]; then
    echo "[信息] 运行: $DEBUG_EXE"
    "$DEBUG_EXE"
elif [ -f "$RELEASE_EXE" ]; then
    echo "[信息] 运行: $RELEASE_EXE"
    "$RELEASE_EXE"
elif [ -f "$FLAT_EXE" ]; then
    echo "[信息] 运行: $FLAT_EXE"
    "$FLAT_EXE"
else
    echo "[错误] 找不到可执行文件 (Debug/Release/zengine)"
fi

echo "[信息] 程序运行完毕"
