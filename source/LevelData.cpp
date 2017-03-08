//
//  LevelData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelData.hpp"

using namespace cugl;

void LevelData::addLevelEntry(std::shared_ptr<LevelEntry> entry){
    _levelEntries.push_back(entry);
}

float LevelData::getTime(int index){
    return _levelEntries.at(index)->time;
}

int LevelData::getWaveKey(int index){
    return _levelEntries.at(index)->waveKey;
}

size_t LevelData::getNumberWaves(){
    return _levelEntries.size();
}

std::string LevelData::serialize(){
    return "";
}

bool LevelData::preload(const std::string& file){
	std::cout << file.c_str() << std::endl;
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool LevelData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	auto entry = LevelEntry::alloc(json->getFloat("waveKey"), json->getInt("time"));
	init(json->getInt("id"));
	addLevelEntry(entry);
    return true;
}

bool LevelData::materialize(){
    return true;
}
