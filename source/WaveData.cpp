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
//#include <iostream>
//#include <fstream>

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

std::string WaveEntry::jsonString()
{
	std::string serialized_string = "\"objectKey\":\"" + objectKey + "\",\n"
		+ "\"aiKey\":" + "\"" + aiKey + "\",\n"
		+ "\"x\":" + std::to_string(position.x * GAME_PHYSICS_SCALE) + ",\n"
		+ "\"y\":" + std::to_string(position.y * GAME_PHYSICS_SCALE) + ",\n"
		+ "\"element\":" + "\"" + std::string((element == Element::BLUE) ? "BLUE" : "GOLD") + "\"\n";
	return serialized_string;
}

std::string WaveData::serialize()
{
	std::string serialized_string = "{\n\"waveEntries\":{\n";
	for (int i = 0; i < _waveEntries.size(); i++)
	{
		auto object = _waveEntries.at(i);
		serialized_string += "\"" + object->objectKey + "\":{" + object->jsonString() + "}\n";
		if (i < _waveEntries.size() - 1) serialized_string += ",";
	}
	serialized_string += "}\n}";
	
    return serialized_string;
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
	//std::string filename = "test_writer.json";
	//Pathname path = Pathname(filename);
	//std::shared_ptr<JsonWriter> writer = JsonWriter::alloc(path);
	//writer->writeJson(json);

	//std::ofstream myfile;
	//myfile.open("test_writer2.json");
	//myfile << this->serialize();
	//myfile.close();
    return true;
}

bool WaveData::materialize(){
    return true;
}
