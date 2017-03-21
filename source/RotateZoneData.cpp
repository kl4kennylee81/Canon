//
//  RotateZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "RotateZoneData.hpp"

using namespace cugl;

std::string RotateZoneData::serialize(){
    return "";
}

bool RotateZoneData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool RotateZoneData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool RotateZoneData::materialize(){
    return true;
}
