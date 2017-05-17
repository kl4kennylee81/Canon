//
//  GameStateEnums.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameStateEnums_hpp
#define GameStateEnums_hpp

#include <stdio.h>
#include <cugl/cugl.h>

enum class GameplayState : int {
    RESET, // game will reset
    TUTORIAL_PAUSE, // tutorial is still playing but rest is paused
    NORMAL // when game is playing normally
};

#endif /* GameStateEnums_hpp */
