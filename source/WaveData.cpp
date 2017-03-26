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

std::string WaveData::serialize(){
    return "";
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
