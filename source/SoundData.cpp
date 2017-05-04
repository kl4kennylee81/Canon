//
//  SoundData.cpp
//  Canon
//
//  Created by Jonathan Chen on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SoundData.hpp"
#include "GameState.hpp"

using namespace cugl;

bool SoundData::init(){
    return true;
}

std::shared_ptr<JsonValue> SoundData::toJsonValue()
{
    std::shared_ptr<JsonValue> soundData = Data::toJsonValue();
    for (auto it : soundMap) {
        soundData->appendChild(actionToString(it.first), JsonValue::alloc(it.second));
    }
    return nullptr;
}

bool SoundData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool SoundData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init();
    for (int i = 0; i < json->size(); i++) {
        auto actionjson = json->get(i);
        
        std::string actionString = actionjson->key();
        auto action = stringToAction(actionString);
        
        std::string soundFxString = actionjson->asString();
        
        soundMap.insert({action,soundFxString});
    }
	Data::preload(json);
    return true;
}

bool SoundData::materialize(){
    return true;
}
