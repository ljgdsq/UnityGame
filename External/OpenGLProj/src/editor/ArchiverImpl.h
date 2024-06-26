//
// Created by zhengshulin on 2024/6/26.
//

#ifndef OPENGLPROJ_ARCHIVERIMPL_H
#define OPENGLPROJ_ARCHIVERIMPL_H
#include "core/Archiver.h"
#include "EditorScene.h"
#include "ui/UISprite.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace rapidjson;
template<>
class Archiver<UISprite>{
    std::string save(const UISprite*obj){
        Document doc;
        doc.SetObject();
        Value sp(kObjectType);

    }
};


template<>
class Archiver<EditorScene>{
    std::string save(const EditorScene*obj){
        auto nodes=obj->getRunningScene()->GetNodes();
        for (const auto node:nodes) {
            if (auto sp=dynamic_cast<UISprite*>(node)){

            }
        }


        return "";
    }

    void load(const EditorScene*obj,std::string&data){

    };
};



#endif //OPENGLPROJ_ARCHIVERIMPL_H
