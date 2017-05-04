//
//  LevelData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelData.hpp"
#include "GameState.hpp"
#include "Util.hpp"


using namespace cugl;

std::shared_ptr<cugl::JsonValue> LevelEntry::toJsonValue(){
    std::shared_ptr<JsonValue> waveDetails = Data::toJsonValue();
    waveDetails->appendChild("waveKey", JsonValue::alloc(waveKey));
    waveDetails->appendChild("time", JsonValue::alloc(time));
    return waveDetails;
}

bool LevelEntry::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool LevelEntry::preload(const std::shared_ptr<cugl::JsonValue>& json){
    Data::preload(json);
    return init(json->getString("waveKey"), json->getInt("time"));
}

bool LevelEntry::materialize(){
    return true;
}

void LevelData::addLevelEntry(std::shared_ptr<LevelEntry> entry){
    _levelEntries.push_back(entry);
}

void LevelData::addPlayerChars(std::shared_ptr<WaveEntry> entry){
    _playerChars.push_back(entry);
}

void LevelData::updateEntryTime(int index, float time){
    _levelEntries.at(index)->time = time;
}

/**
 * index is the wave number of the level
 */
float LevelData::getTime(int index){
    return _levelEntries.at(index)->time;
}

std::string LevelData::getWaveKey(int index){
    return _levelEntries.at(index)->waveKey;
}

std::shared_ptr<LevelEntry> LevelData::getLevelEntry(int index){
    return _levelEntries.at(index);
}

size_t LevelData::getNumberWaves(){
    return _levelEntries.size();
}

std::shared_ptr<JsonValue> LevelData::toJsonValue(){

	std::shared_ptr<JsonValue> levelList = Data::toJsonValue();
	for (int i = 0; i < getNumberWaves(); i++)
	{
		std::shared_ptr<JsonValue> waveDetails = JsonValue::allocObject();
		waveDetails->appendChild("waveKey", JsonValue::alloc(getWaveKey(i)));
		waveDetails->appendChild("time", JsonValue::alloc(getTime(i)));
        // levelEntry key
        levelList->appendChild(_levelEntries.at(i)->key, waveDetails);
	}

	std::shared_ptr<JsonValue> playerList = JsonValue::allocObject();
	for (int i = 0; i < getPlayerChars().size(); i++)
	{
		playerList->appendChild("player" + std::to_string(i + 1), getPlayerChars().at(i)->toJsonValue());
	}

	std::shared_ptr<JsonValue> finalList = JsonValue::allocObject();
	finalList->appendChild("levelEntries", levelList);
	finalList->appendChild("playerChars", playerList);
	return finalList;
}

bool LevelData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool LevelData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::shared_ptr<JsonValue> levelEntries = json->get("levelEntries");
	for (int i = 0; i < levelEntries->size(); i++) {
		auto child = levelEntries->get(i);
		auto entry = LevelEntry::alloc(child);
		addLevelEntry(entry);
	}
    // get the blue player character
    std::shared_ptr<JsonValue> playerChars = json->get("playerChars");
    for (int i = 0; i < playerChars->size(); i++){
        auto child = playerChars->get(i);
        auto entry = WaveEntry::alloc(child);
        this->addPlayerChars(entry);
    }
    
    init();
	Data::preload(json);
    return true;
}

bool LevelData::materialize(){
    return true;
}

bool LevelData::isValid(){
    // check that there are two player characters
    if (!(_playerChars.size() == 2)){
        return false;
    }
    return true;
}
