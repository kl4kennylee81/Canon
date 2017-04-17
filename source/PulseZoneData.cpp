//
//  PulseZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PulseZoneData.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> PulseZoneData::toJsonValue(){
	std::shared_ptr<JsonValue> pz = JsonValue::allocObject();
	pz->appendChild("type", JsonValue::alloc("PULSE"));
	pz->appendChild("objectKey", JsonValue::alloc(objectKey));
	pz->appendChild("minSize", JsonValue::alloc(minSize));
	pz->appendChild("minTime", JsonValue::alloc(static_cast<float>(minTime)));
	pz->appendChild("maxSize", JsonValue::alloc(maxSize));
	pz->appendChild("maxTime", JsonValue::alloc(static_cast<float>(maxTime)));
	pz->appendChild("speed", JsonValue::alloc(speed));
    pz->appendChild("element", JsonValue::alloc(static_cast<std::string>(elementDataTypeToString(this->elementType))));
	return pz;
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
    auto el = stringToElementDataType(json->getString("element"));
    init(oid,minSize,minTime,maxSize,maxTime,speed,el);

    return true;
}

bool PulseZoneData::materialize(){
    return true;
}
