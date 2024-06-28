//
// Created by zhengshulin on 2024/6/27.
//

#include "Canvas.h"
#include "glm/ext/matrix_transform.hpp"

std::vector<CanvasItem *> Canvas::childs;
std::queue<CanvasDrawCommand*> Canvas::rendererQueue;

void Canvas::Process(const Context&ctx) {
    for (const auto &item: childs) {
        if (item->active)
            item->Process(ctx);
    }

}

void Canvas::AddCanvasItem(CanvasItem *item) {
    childs.push_back(item);
}

void Canvas::RemoveCanvasItem(CanvasItem *item) {
    auto it = std::find(childs.begin(), childs.end(), item);
    if (it != childs.end()) {
        childs.erase(it);
    }
}

std::string Canvas::GetType() {
    return "Canvas";
}
#define MAX_SPRITE 300

void Canvas::OnCreate() {
    vao=new VertexArray();
    vbo=new Buffer(BufferType::VBO);
    ebo=new Buffer(BufferType::EBO);

    vbo->uploadData(MAX_SPRITE* sizeof(Vertex)*6, nullptr,DataUsage::STATIC_DRAW);
    ebo->uploadData(MAX_SPRITE* sizeof(unsigned int)*6, nullptr,DataUsage::STATIC_DRAW);
    vao->addBufferLayout({0,3,GL_FLOAT, false,5* sizeof(float ),0});
    vao->addBufferLayout({1,2,GL_FLOAT, false,5* sizeof(float ),3* sizeof(float )});
    vao->enableAttribute(0);
    vao->enableAttribute(1);

}

void Canvas::OnEnable() {

}

void Canvas::OnDisable() {

}

void Canvas::OnDestroy() {

}

Canvas::Canvas(GameObject *gameObject) : CanvasItem(gameObject) {
    quadBatch.shaderID=-1;
    quadBatch.texID=-1;
    quadBatch.vertices.clear();
    quadBatch.indices.clear();
}

void Canvas::SubmitDrawCommand(CanvasDrawCommand *command) {
    rendererQueue.push(command);
}


glm::vec3 applyTransform(const glm::vec3 &pos, const glm::mat4 &transform) {
    glm::vec4 transformedPos = transform * glm::vec4(pos, 1.0f);
    return glm::vec3(transformedPos);
}

#include <glm/glm.hpp>

void transformVertices(std::vector<Vertex> &vertices, const glm::vec3 &position, const glm::vec3 &rotate,
                       const glm::vec3 &scale) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    for (auto &vertex: vertices) {
        vertex.position = applyTransform(vertex.position, model);
    }
}


void Canvas::Draw() {
    if (rendererQueue.empty()) return;
    vao->bind();
    int indexOffset=0;
    while (rendererQueue.size()>0) {
        auto command = rendererQueue.front();
        rendererQueue.pop();
        if (command->GetType() == Quad) {
            FlushOther();
            std::vector<Vertex> ndc = {
                    {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}},  // 左上角
                    {{0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f}},  // 右下角
                    {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},  // 左下角

                    {{-0.5f, 0.5f,  0.0f}, {0.0f, 1.0f}},  // 左上角
                    {{0.5f,  0.5f,  0.0f}, {1.0f, 1.0f}},  // 右上角
                    {{0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f}}   // 右下角
            };

            unsigned int indices[] = {
                    0,  1,  2,
                    3, 4, 5
            };

            for (int i = 0; i < 6; ++i) {
                indices[i]=indices[i]+indexOffset;
            }
            indexOffset+=6;


            auto quadComm = dynamic_cast<QuadDrawCommand *>(command);
            if ((quadBatch.shaderID == quadComm->shaderID && quadBatch.texID == quadComm->texID) && quadBatch.vertices.size()>0) {

                transformVertices(ndc, quadComm->position, quadComm->rotate, quadComm->scale * vec3(quadComm->size, 1));
                quadBatch.vertices.insert(quadBatch.vertices.end(), std::begin(ndc), std::end(ndc));
                quadBatch.indices.insert(quadBatch.indices.end(), std::begin(indices), std::end(indices));
            } else {
                FlushQuad();
                quadBatch.shaderID=quadComm->shaderID;
                quadBatch.texID=quadComm->texID;
                transformVertices(ndc, quadComm->position, quadComm->rotate, quadComm->scale * vec3(quadComm->size, 1));
                quadBatch.vertices.insert(quadBatch.vertices.end(), std::begin(ndc), std::end(ndc));
                quadBatch.indices.insert(quadBatch.indices.end(), std::begin(indices), std::end(indices));
            }

        } else {
            FlushQuad();
        }
    }

    FlushQuad();
    FlushOther();
}

void Canvas::FlushQuad() {
    if (quadBatch.indices.size()==0)return;
    batchCount=quadBatch.indices.size()/6;
    glUseProgram(quadBatch.shaderID);
    vao->bind();
    vbo->bind();

//    glBufferSubData(GL_ARRAY_BUFFER,0,quadBatch.vertices.size()* sizeof(Vertex),quadBatch.vertices.data());

    vbo->uploadData(quadBatch.vertices.size()* sizeof(Vertex),quadBatch.vertices.data(),DataUsage::STATIC_DRAW);
    ebo->uploadData(quadBatch.indices.size()* sizeof(unsigned int),quadBatch.indices.data(),DataUsage::STATIC_DRAW);
//    vao->addBufferLayout({0,3,GL_FLOAT, false,5* sizeof(float ),0});
//    vao->addBufferLayout({1,2,GL_FLOAT, false,5* sizeof(float ),3* sizeof(float )});
//    vao->enableAttribute(0);
//    vao->enableAttribute(1);

    const GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        printf("*** OpenGL error after make current: 0x%08x ***\n", error);


 glDrawElements(GL_TRIANGLES, 6*batchCount, GL_UNSIGNED_INT, 0);
//glDrawArrays(GL_TRIANGLES,0,quadBatch.vertices.size());
    quadBatch.indices.clear();
    quadBatch.vertices.clear();

}

void Canvas::FlushOther() {

}

bool Canvas::isSameBatch(QuadDrawCommand *command) {
    return false;
}

CanvasItem::CanvasItem(GameObject *gameObject) : Component(gameObject) {}

CanvasDrawType QuadDrawCommand::GetType() const {
    return Quad;
}
