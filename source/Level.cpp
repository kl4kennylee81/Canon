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
    return true;
}

int Level::getNextTime(){
    return _levelData->getTime(_currentWave);
}

int Level::getCurrentWaveKey(){
    return _levelData->getWaveKey(_currentWave);
}

int Level::getCurrentWave(){
    return _currentWave;
}

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
    return (currentTime+_timeElapsed)/allTime;
}

/** return -1 if not ready to spawn the wave. If it is ready returns the current wave key 
 *  after returning it will not return the wave key until the next wave. To refer to the
 *  active wave key call getCurrentWaveKey
 */
int Level::pollWave() {
    if (_readyToSpawn){
        _readyToSpawn = false;
        return getCurrentWaveKey();
    } else {
        return -1;
    }
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
