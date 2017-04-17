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
    
    if (isLastWave()){
        _readyToSpawn = false;
    } else {
        _readyToSpawn = true;
    }
    
    return true;
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
    return _framesElapsed/((float)getNextTime());
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
