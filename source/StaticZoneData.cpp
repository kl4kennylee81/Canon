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
	std::shared_ptr<JsonValue> data = JsonValue::allocObject();
	data->appendChild("type", JsonValue::alloc("STATIC"));
	data->appendChild("objectKey", JsonValue::alloc(objectKey));
	data->appendChild("relX", JsonValue::alloc(relPos.x));
	data->appendChild("relY", JsonValue::alloc(relPos.y));
	data->appendChild("cooldown", JsonValue::alloc(static_cast<float>(cooldown)));
	data->appendChild("duration", JsonValue::alloc(static_cast<float>(duration)));
	data->appendChild("element", JsonValue::alloc((element == Element::BLUE) ? "BLUE" : "GOLD"));
	return data->toString();
}


bool StaticZoneData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool StaticZoneData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string oid = json->getString("objectKey");
    float rel_x = json->getFloat("relX");
    float rel_y = json->getFloat("relY");
    int cooldown = json->getInt("cooldown");
    int duration = json->getInt("duration");
    auto el = json->getString("Element").compare("BLUE") ? Element::BLUE : Element::GOLD;
    init(oid,rel_x,rel_y,cooldown,duration,el);

    return true;
}

bool StaticZoneData::materialize(){
    return true;
}
