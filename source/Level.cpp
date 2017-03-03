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

void Level::update(float timestep){
    _timeElapsed+=1;
    if (!isLastWave() && _timeElapsed >= getNextTime()){
        std::cout<< "current wave is:" << _currentWave << std::endl;
        std::cout << "Number wave is:" << this->_levelData->getNumberWaves() << std::endl;
        _timeElapsed = 0;
        if (_levelData->getNumberWaves() - 1 > _currentWave){
            _readyToSpawn = true;
            _currentWave+=1;
            
        }
    };
};

void Level::reset(){
    init(_levelData);
};
