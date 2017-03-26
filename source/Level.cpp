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
    _timeElapsed = 0;
    _readyToSpawn = true;
    _playerSpawned = false;
    return true;
}

int Level::getNextTime(){
    return _levelData->getTime(_currentWave);
}

std::string Level::getCurrentWaveKey(){
    return _levelData->getWaveKey(_currentWave);
}

int Level::getCurrentWave(){
    return _currentWave;
}

float Level::getProgress(){
    return _timeElapsed/((float)getNextTime());
}

bool Level::isReadyToSpawn(){
    return _readyToSpawn;
}

void Level::toggleReadyToSpawn(){
    this->_readyToSpawn = !this->_readyToSpawn;
}

bool Level::hasPlayerSpawned(){
    return this->_playerSpawned;
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
    return _currentWave >= _levelData->getNumberWaves() - 1;
}

bool Level::isSpawningFinished(){
    return isLastWave() && (_timeElapsed == getNextTime());
}

void Level::update(float timestep){
    if (_timeElapsed < getNextTime()){
        _timeElapsed+=1;
        return;
    }
    
    if (!isLastWave()){
        _timeElapsed = 0;
        _readyToSpawn = true;
        _currentWave+=1;
    }
}

void Level::reset(){
    init(_levelData);
}
