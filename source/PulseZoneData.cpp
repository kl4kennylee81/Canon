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
    std::string oid = json->getString("objectKey");
    float minSize = json->getFloat("minSize");
    int minTime = json->getInt("minTime");
    float maxSize = json->getFloat("maxSize");
    int maxTime = json->getInt("maxTime");
    float speed = json->getFloat("speed");
    auto el = json->getString("element").compare("BLUE") == 0 ? Element::BLUE : Element::GOLD;
    init(oid,minSize,minTime,maxSize,maxTime,speed,el);
    return true;
}

bool PulseZoneData::materialize(){
    return true;
}
