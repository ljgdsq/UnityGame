#pragma once
#include "Framework/Common/Define.h"
#include <string>
namespace framework
{
    class Project
    {
        NON_COPY_AND_MOVE(Project);
        public:
            Project() = default;
            ~Project() = default;

            // 初始化项目
            void Initialize();

            // 获取项目名称
            const std::string &GetName() const { return m_name; }

            // 设置项目名称
            void SetName(const std::string &name) { m_name = name; }
        private:
            std::string m_name;
    };
}
