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
public:
    Level(){}
    
    bool init(std::shared_ptr<LevelData> levelData);
    
    int getNextTime();
    
    void setCurrentWave(int waveNum);

    bool isLastWave();
    
    void update(float timestep);
};

#endif /* Level_hpp */
