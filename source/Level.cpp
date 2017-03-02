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
    return true;
}

int Level::getNextTime(){
    return _levelData->getTime(_currentWave);
}

void Level::setCurrentWave(int waveNum){
    _currentWave = waveNum;
}

bool Level::isLastWave(){
    return _currentWave == _levelData ->getNumberWaves();
}

void Level::update(float timestep){
    _timeElapsed+=timestep;
    if (_timeElapsed >= getNextTime() && !isLastWave()){
        _timeElapsed = 0;
        if (_levelData->getNumberWaves() < _currentWave - 1){
            _currentWave+=1;
        }
    };
};
