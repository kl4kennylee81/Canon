//
//  Level.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <list>
#include "LevelData.hpp"


/**
 * Abstracts away the spawning and keeping track of total time
 */
class Level {
protected:
    std::shared_ptr<LevelData> _levelData;
    /** current index of the wave */
    int _currentWave;
    /** time elapsed since last wave */
    float _framesElapsed;
    bool _readyToSpawn;
    bool _playerSpawned;
public:
    Level(){}

	bool isReadyToSpawn() { return _readyToSpawn; }
	bool hasPlayerSpawned() { return _playerSpawned; }
	std::string getLevelKey() { return _levelData->key; }
	float getFramesElapsed() { return _framesElapsed; }
    
    void setLevelData(std::shared_ptr<LevelData> levelData){
        _levelData = levelData;
    }
    
    bool init(std::shared_ptr<LevelData> levelData);
    
    bool init(int currentWave,int framesElapsed,bool readyToSpawn,bool playerSpawned);
    
    bool init(const std::shared_ptr<cugl::JsonValue> json);
    
    int getNextTime();
    
    std::string getCurrentWaveKey();
    
    /** returns the current wave and -1 if there are no waves */
    int getCurrentWave();
    
    float getProgress();
    
    void toggleReadyToSpawn();
    
    void togglePlayerSpawned();
    
    std::vector<std::shared_ptr<WaveEntry>> getPlayerChars();
    
    void setCurrentWave(int waveNum);

    bool isLastWave();
    
    bool isSpawningFinished();
    
    void update(float timestep);
    
    void reset();
};

#endif /* Level_hpp */
