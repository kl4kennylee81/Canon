//
//  ZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/20/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ZoneData.hpp"

using namespace cugl;

std::string ZoneData::serialize(){
    return "";
}

bool ZoneData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool ZoneData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool ZoneData::materialize(){
    return true;
}
