//
//  Level.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Level.hpp"

using namespace cugl;

bool Level::init(std::shared_ptr<LevelData> levelData){
    _levelData = levelData;
    _currentWave = 0;
    _framesElapsed = 0;
    _playerSpawned = false;
    _readyToSpawn = false;
    
    if (_levelData == nullptr){
        return true;
    }
    
    if (isLastWave()){
        _readyToSpawn = false;
    } else {
        _readyToSpawn = true;
    }
    
    return true;
}

bool Level::init(int currentWave,int framesElapsed,bool readyToSpawn,bool playerSpawned){
    _currentWave = currentWave;
    _framesElapsed = framesElapsed;
    _readyToSpawn = readyToSpawn;
    _playerSpawned = playerSpawned;
    return true;
}

bool Level::init(const std::shared_ptr<JsonValue> json){
    return init(json->getInt("currentWaveIndex"),
                 json->getInt("framesElapsed"),
                 json->getBool("readyToSpawn"),
                 json->getBool("playerSpawned"));
}

/**
 * Gets the frames of when the next wave is supposed to spawn
 */
int Level::getNextTime(){
    return _levelData->getTime(_currentWave);
}

std::string Level::getCurrentWaveKey(){
    return _levelData->getWaveKey(_currentWave);
}

int Level::getCurrentWave(){
    if (_levelData->getNumberWaves() == 0){
        return -1;
    }
    return _currentWave;
}

/**
 * Gets percentage of how much of the wave is finished
 */
float Level::getProgress(){
    float allTime = 0;
    float currentTime = 0;
    // add up all previous waves times
    for (int i =0;i<_levelData->getNumberWaves();i++){
        if (i < getCurrentWave()){
            currentTime+=_levelData->getTime(i);
        }
        allTime+=_levelData->getTime(i);
    }
    // time elapsed is how much in the current wave
    return (currentTime+_framesElapsed)/allTime;
}

void Level::toggleReadyToSpawn(){
    this->_readyToSpawn = !this->_readyToSpawn;
}

std::vector<std::shared_ptr<WaveEntry>> Level::getPlayerChars(){
    return this->_levelData->getPlayerChars();
}

void Level::togglePlayerSpawned(){
    this->_playerSpawned = !this->_playerSpawned;
}

void Level::setCurrentWave(int waveNum){
    _currentWave = waveNum;
}


bool Level::isLastWave(){
    return _currentWave >= (int)_levelData->getNumberWaves();
}

bool Level::isSpawningFinished(){
    return getCurrentWave() < 0 || // the level has no waves
    (_currentWave ==_levelData->getNumberWaves());
}

void Level::update(float timestep){
    
    if (isLastWave()){
        return;
    }
    
    if (_framesElapsed < getNextTime()){
        _framesElapsed += GameState::_internalClock->getTimeDilation();
        return;
    }
    _framesElapsed = 0; // reset frames for next waves
    _currentWave+=1;
    if (!isLastWave()){
        _readyToSpawn = true;
    }
}

void Level::reset(){
    init(_levelData);
}
