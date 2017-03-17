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

class Level {
protected:
    std::shared_ptr<LevelData> _levelData;
    /** current index of the wave */
    int _currentWave;
    /** time elapsed since last wave */
    float _timeElapsed;
    bool _readyToSpawn;
public:
    Level(){}
    
    bool init(std::shared_ptr<LevelData> levelData);
    
    int getNextTime();
    
    int getCurrentWaveKey();
    
    int getCurrentWave();
    
    float getProgress();
    
    /** return -1 if not ready to spawn the wave. If it is ready returns the current wave key 
     *  after returning it, it will return -1 again until the next wave */
    int pollWave();
    
    void setCurrentWave(int waveNum);

    bool isLastWave();
    
    bool isSpawningFinished();
    
    void update(float timestep);
    
    void reset();
};

#endif /* Level_hpp */
