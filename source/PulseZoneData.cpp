//
//  PulseZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PulseZoneData.hpp"

using namespace cugl;

std::string PulseZoneData::serialize(){
    return "";
}

bool PulseZoneData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool PulseZoneData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool PulseZoneData::materialize(){
    return true;
}
