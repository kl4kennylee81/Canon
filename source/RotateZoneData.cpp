//
//  RotateZoneData.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "RotateZoneData.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> ZoneEntry::toJsonValue()
{
	std::shared_ptr<JsonValue> z = JsonValue::allocObject();
	z->appendChild("objectKey", JsonValue::alloc(objectKey));
	z->appendChild("startingPosition", JsonValue::alloc(startingPosition));
    z->appendChild("element", JsonValue::alloc(Element::elementDataTypeToString(this->elementType)));
	return z;
}

cugl::Vec2 ZoneEntry::getPosition(cugl::Vec2 objPos,float radius){
    float angle = getAngle();
    cugl::Vec2 offset;
    offset.x = - (sin(angle));
    offset.y = cos(angle);
    offset *= radius;
    return objPos+offset;
}

float ZoneEntry::getAngle(){
    return this->startingPosition*2*M_PI;
}

std::shared_ptr<JsonValue> RotateZoneData::toJsonValue(){
	std::shared_ptr<JsonValue> data = Data::toJsonValue();
	data->appendChild("type", JsonValue::alloc("ROTATE"));
	std::shared_ptr<JsonValue> zoneEntries = JsonValue::allocObject();

	for (int i = 0; i < zones.size(); i++)
	{
		zoneEntries->appendChild("zone" + std::to_string(i + 1), zones.at(i)->toJsonValue());
	}

	data->appendChild("zoneEntries", zoneEntries);
	data->appendChild("radius", JsonValue::alloc(radius));
	data->appendChild("speed", JsonValue::alloc(speed));
	return data;
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
        ElementDataType el = Element::stringToElementDataType(zonejson->getString("element"));
        std::shared_ptr<ZoneEntry> entry = ZoneEntry::alloc(oKey,startingPos,el);
        zones.push_back(entry);
    }

	Data::preload(json);
    return true;
}

bool RotateZoneData::materialize(){
    return true;
}
