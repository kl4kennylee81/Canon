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

bool WaveEntry::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string tempKey = json->getString("templateKey");
    
    init(json->getFloat("x"),
        json->getFloat("y"),
        json->getString("element") == "BLUE" ? ElementType::BLUE : ElementType::GOLD,
        json->getString("templateKey"),
        json->getString("aiKey"));
    return true;
}

bool WaveEntry::init(float x, float y,ElementType element,std::string templateKey, std::string aiKey){
    Data::init();
    this->position.x = x / GAME_PHYSICS_SCALE;
    this->position.y = y / GAME_PHYSICS_SCALE;
    this->element = element;
    this->templateKey = templateKey;
    this->aiKey = aiKey;
    return true;
}

std::shared_ptr<JsonValue> WaveEntry::toJsonValue()
{
	std::shared_ptr<JsonValue> object = JsonValue::allocObject();
	object->appendChild("x", JsonValue::alloc(position.x * GAME_PHYSICS_SCALE));
	object->appendChild("y", JsonValue::alloc(position.y * GAME_PHYSICS_SCALE));
	object->appendChild("element", JsonValue::alloc((element == ElementType::BLUE) ? "BLUE" : "GOLD"));
    object->appendChild("aiKey", JsonValue::alloc(aiKey));
    if(templateKey != ""){
        object->appendChild("templateKey", JsonValue::alloc(templateKey));
    }
	
	std::shared_ptr<JsonValue> zones = JsonValue::allocArray();
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
	Data::preload(json);
    return true;
}

bool WaveData::materialize(){
    return true;
}

ElementType WaveEntry::getElement(){
    return element;
}

cugl::Vec2 WaveEntry::getPosition(){
    return position;
}

std::string WaveEntry::getTemplateKey(){
    return templateKey;
}

void WaveEntry::setTemplateKey(std::string tKey){
    this->templateKey = tKey;
}

void WaveEntry::setPosition(cugl::Vec2 pos){
    this->position = pos;
}

