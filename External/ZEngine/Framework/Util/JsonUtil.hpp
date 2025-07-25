#pragma once
#include <string>
#include "Framework/Common/Define.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "Framework/Core/EngineFileIO.h"
namespace framework
{

    class JsonUtil final
    {
        STATIC_CLASS(JsonUtil);

    public:
        // 将JSON对象转换为字符串
        static std::string JsonToString(const rapidjson::Value &jsonValue)
        {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            jsonValue.Accept(writer);
            return buffer.GetString();
        }

        // 将字符串转换为JSON对象
        static rapidjson::Value StringToJson(const std::string &jsonString, rapidjson::Document::AllocatorType &allocator)
        {
            rapidjson::Document doc;
            doc.Parse(jsonString.c_str());
            return doc;
        }
    };
}
