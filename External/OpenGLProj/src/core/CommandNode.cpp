//
// Created by zhengshulin on 2024/6/21.
//

#include "CommandNode.h"
#include "EmptyDrawable.h"

void CommandNode::UpdateShader(const Context &ctx) {
    if (DrawCommand)
        DrawCommand(ctx);
}

CommandNode::CommandNode() {
}
