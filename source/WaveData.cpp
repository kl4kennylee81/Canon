//
//  WaveData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "WaveData.hpp"
#include "AIData.hpp"
#include "GameState.hpp" // for conversion to physicsScale
#include "Element.hpp"

using namespace cugl;

bool WaveEntry::init(const std::shared_ptr<cugl::JsonValue>& json){
    std::vector<std::string> zKeys;
    if (json->has("zoneKeys")) {
        zKeys = json->get("zoneKeys")->asStringArray();
    }
    init(json->getString("objectKey"),
        json->getString("aiKey"),
        json->getFloat("x"),
        json->getFloat("y"),
        json->getString("element") == "BLUE" ? Element::BLUE : Element::GOLD,
        zKeys);
    if(json->has("templateKey")) {
        templateKey = json->getString("templateKey");
    }
    return true;
}

bool WaveEntry::init(std::string objectKey, std::string aiKey, float x, float y,Element element,std::vector<std::string> zoneKeys){
    this->objectKey = objectKey;
    this->aiKey = aiKey;
    this->position.x = x / GAME_PHYSICS_SCALE;
    this->position.y = y / GAME_PHYSICS_SCALE;
    this->element = element;
    this->zoneKeys = zoneKeys;
    return true;
}

std::shared_ptr<JsonValue> WaveEntry::toJsonValue()
{
	std::shared_ptr<JsonValue> object = JsonValue::allocObject();
	if (aiKey.length() > 0) object->appendChild("aiKey", JsonValue::alloc(aiKey));
	object->appendChild("x", JsonValue::alloc(position.x * GAME_PHYSICS_SCALE));
	object->appendChild("y", JsonValue::alloc(position.y * GAME_PHYSICS_SCALE));
	object->appendChild("element", JsonValue::alloc((element == Element::BLUE) ? "BLUE" : "GOLD"));
	object->appendChild("objectKey", JsonValue::alloc(objectKey));
    if(templateKey != ""){
        object->appendChild("templateKey", JsonValue::alloc(templateKey));
    }
	
	std::shared_ptr<JsonValue> zones = JsonValue::allocArray();
	for (int i = 0; i < zoneKeys.size(); i++)
	{
		zones->appendChild(JsonValue::alloc(zoneKeys.at(i)));
	}
	if (zoneKeys.size() > 0) object->appendChild("zoneKeys", zones);
	return object;
}

std::shared_ptr<JsonValue> WaveData::toJsonValue()
{
	std::shared_ptr<JsonValue> wave = JsonValue::allocObject();
	std::shared_ptr<JsonValue> objectList = JsonValue::allocObject();
	for (int i = 0; i < _waveEntries.size(); i++)
	{
		auto child = _waveEntries.at(i);
		std::shared_ptr<JsonValue> object = child->toJsonValue();
		objectList->appendChild("object" + std::to_string(i + 1), object);		
	}
	wave->appendChild("waveEntries", objectList);
	return wave;
}

bool WaveData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool WaveData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init();
    std::shared_ptr<JsonValue> waveEntries = json->get("waveEntries");
	for (int i = 0; i < waveEntries->size(); i++) {
		auto child = waveEntries->get(i);
		auto entry = WaveEntry::alloc(child);
		addWaveEntry(entry);
	}
    return true;
}

bool WaveData::materialize(){
    return true;
}
