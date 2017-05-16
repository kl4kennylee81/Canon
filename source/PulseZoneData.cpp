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
	std::shared_ptr<JsonValue> pz = Data::toJsonValue();
	pz->appendChild("type", JsonValue::alloc("PULSE"));
	pz->appendChild("objectKey", JsonValue::alloc(objectKey));
	pz->appendChild("minSizeX", JsonValue::alloc(minSize.x));
	pz->appendChild("minSizeY", JsonValue::alloc(minSize.y));
	pz->appendChild("minTime", JsonValue::alloc(static_cast<float>(minTime)));
	pz->appendChild("maxSizeX", JsonValue::alloc(maxSize.x));
	pz->appendChild("maxSizeY", JsonValue::alloc(maxSize.y));
	pz->appendChild("maxTime", JsonValue::alloc(static_cast<float>(maxTime)));
	pz->appendChild("lerpTime", JsonValue::alloc(_lerpTime));
    pz->appendChild("element", JsonValue::alloc(static_cast<std::string>(Element::elementDataTypeToString(this->elementType))));
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
    float minSizeX = json->getFloat("minSizeX",1.0);
    float minSizeY = json->getFloat("minSizeY",1.0);
    Vec2 minSize = Vec2::Vec2(minSizeX,minSizeY);
    int minTime = json->getInt("minTime");
    
    float maxSizeX = json->getFloat("maxSizeX",1.0);
    float maxSizeY = json->getFloat("maxSizeY",1.0);
    Vec2 maxSize = Vec2::Vec2(maxSizeX,maxSizeY);
    
    int maxTime = json->getInt("maxTime");
    float lerpTime = json->getFloat("lerpTime");
    auto el = Element::stringToElementDataType(json->getString("element"));
    init(oid,minSize,minTime,maxSize,maxTime,lerpTime,el);

	Data::preload(json);
    return true;
}

bool PulseZoneData::materialize(){
    return true;
}

cugl::Vec2 PulseZoneData::getPosition(cugl::Vec2 objPos){
    return objPos;
}

cugl::Vec2 PulseZoneData::getLerpSpeed(){
    return (maxSize - minSize)/_lerpTime;
}
