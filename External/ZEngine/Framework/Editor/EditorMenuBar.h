#pragma once
#include "Framework/Editor/EditorWidget.h"
namespace editor {
    class EditorMenuBar: public EditorWidget{

    public:
        EditorMenuBar(const std::string &name = "Editor Menu Bar")
            : EditorWidget(name) {}
            
        void Initialize() override;

        void Update(float deltaTime) override;

        void Render() override;

        void Shutdown() override;
    };

}



