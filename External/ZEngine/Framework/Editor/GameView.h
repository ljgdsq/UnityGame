#pragma once
#include "Framework/Editor/EditorWidget.h"
#include "Framework/Render/Buffer.h"
namespace editor{
    class GameView : public EditorWidget
    {
    public:
        GameView();
        ~GameView() override;

        void Initialize() override;
        void Render() override;
        void Update(float deltaTime) override;
        void Shutdown() override;

        public:
            void SetGameFrameBuffer(FrameBuffer *frameBuffer);

        private:
            FrameBuffer *m_gameFrameBuffer; // 游戏视图的帧缓冲
            bool m_isFocused = false;       // 是否获得焦点
    };
}