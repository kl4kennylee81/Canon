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
    int id = json->getInt("id");
    int sid = json->getInt("shape_id");
    int aid = json->getInt("animation_id");
    float rel_x = json->getFloat("rel_x");
    float rel_y = json->getFloat("rel_y");
    int cooldown = json->getInt("cooldown");
    int duration = json->getInt("duration");
    auto el = json->getString("element").compare("BLUE") ? Element::BLUE : Element::GOLD;
    init(id,sid,aid,rel_x,rel_y,cooldown,duration,el);
    return true;
}

bool StaticZoneData::materialize(){
    return true;
}
