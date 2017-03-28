//
//  LevelData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelData.hpp"
#include "GameState.hpp"

using namespace cugl;

void LevelData::addLevelEntry(std::shared_ptr<LevelEntry> entry){
    _levelEntries.push_back(entry);
}

void LevelData::addPlayerChars(std::shared_ptr<WaveEntry> entry){
    _playerChars.push_back(entry);
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

size_t LevelData::getNumberWaves(){
    return _levelEntries.size();
}

std::string LevelData::serialize(){
    return "";
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
		auto entry = LevelEntry::alloc(child->getString("waveKey"), child->getInt("time"));
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
    return true;
}

bool LevelData::materialize(){
    return true;
}
