//
//  LevelData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelData.hpp"

using namespace cugl;

void LevelData::addLevelEntry(LevelEntry entry){
    _levelEntries.push_back(entry);
}

float LevelData::getTime(int index){
    return _levelEntries.at(index).time;
}

int LevelData::getWaveKey(int index){
    return _levelEntries.at(index).waveKey;
}

size_t LevelData::getNumberWaves(){
    return _levelEntries.size();
}

std::string LevelData::serialize(){
    return "";
}

bool LevelData::preload(const std::string& file){
    return true;
}

bool LevelData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool LevelData::materialize(){
    return true;
}
