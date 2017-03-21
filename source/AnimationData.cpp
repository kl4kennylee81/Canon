//
//  AnimationData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationData.hpp"

using namespace cugl;

std::string AnimationData::serialize(){
    return "";
}

bool AnimationData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool AnimationData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init(json->getInt("id"));
    texture = Texture::allocWithFile(json->getString("texture"));
    rows = json->getInt("rows");
    cols = json->getInt("cols");
    size = json->getInt("size");
    auto statemapjson = json->get("statemap");
    for (int i = 0; i < statemapjson->size(); i++) {
        auto statejson = statemapjson->get(i);
        std::string state = statejson->key();
        int first = statejson->getInt("first");
        int total = statejson->getInt("total");
        std::vector<int> frames = statejson->get("frames")->asIntArray();
        auto animationstate = AnimationState::alloc(first,total,frames);
        _statemap.insert({state,animationstate});
    }
    
    auto actionmapjson = json->get("actionmap");
    for (int i = 0; i < actionmapjson->size(); i++) {
        auto actionjson = actionmapjson->get(i);
        
        std::string actionString = actionjson->key();
        auto action = stringToAction(actionString);
        
        std::string active;
        if (actionjson->has("active")){
            active = actionjson->getString("active");
        }
        std::string repeat;
        if (actionjson->has("repeat")){
            repeat = actionjson->getString("repeat");
        }
        auto animationupdate = AnimationUpdate::alloc(active, repeat);
        
        _actionmap.insert({action,animationupdate});
    }
    return true;
}

bool AnimationData::materialize(){
    return true;
}
