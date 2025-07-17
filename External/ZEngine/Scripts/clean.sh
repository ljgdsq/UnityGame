#!/bin/bash
# ===============================
# ZEngine Clean Script (Linux/macOS)
# ===============================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$PROJECT_ROOT"

if [ -d "build" ]; then
    echo "[信息] 删除 build 目录..."
    rm -rf build
    echo "[信息] build 目录已删除。"
else
    echo "[信息] 未找到 build 目录，无需清理。"
fi

# 可扩展：清理其他临时文件
# ...

echo "[信息] 清理完成。"
