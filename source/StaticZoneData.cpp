//
//  StaticZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "StaticZoneData.hpp"

using namespace cugl;

std::string StaticZoneData::serialize(){
    return "";
}

bool StaticZoneData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool StaticZoneData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool StaticZoneData::materialize(){
    return true;
}