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
    init(json->getFloat("radius"),json->getFloat("speed"));
    auto zoneentriesjson = json->get("zoneEntries");
    for (int i = 0; i < zoneentriesjson->size(); i++) {
        auto zonejson = zoneentriesjson->get(i);
        std::string oKey = zonejson->getString("objectKey");
        float startingPos = zonejson->getFloat("startingPosition");
        auto el = zonejson->getString("element") == "BLUE" ? Element::BLUE : Element::GOLD;
        std::shared_ptr<ZoneEntry> entry = ZoneEntry::alloc(oKey,startingPos,el);
        zones.push_back(entry);
    }
    return true;
}

bool RotateZoneData::materialize(){
    return true;
}
