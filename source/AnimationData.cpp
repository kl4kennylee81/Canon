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
    
    auto eventmapjson = json->get("eventmap");
    for (int i = 0; i < eventmapjson->size(); i++) {
        auto eventjson = eventmapjson->get(i);
        
        std::string eventString = eventjson->key();
        auto event = stringToEvent(eventString);
        
        std::string active;
        if (eventjson->has("active")){
            active = eventjson->getString("active");
        }
        std::string repeat;
        if (eventjson->has("repeat")){
            repeat = eventjson->getString("repeat");
        }
        auto animationupdate = AnimationUpdate::alloc(active, repeat);
        
        _eventmap.insert({event,animationupdate});
    }
    return true;
}

bool AnimationData::materialize(){
    return true;
}
