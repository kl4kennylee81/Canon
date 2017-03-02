//
//  LevelEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelEvent_hpp
#define LevelEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"
#include "ObjectData.hpp"

class LevelEvent : Event {
    enum class LevelEventType : int {
        /** Signal that a path is done */
        OBJECT_SPAWN
    };
    
    LevelEventType levelEventType;
};

class ObjectSpawnEvent : LevelEvent {
    int objDataKey;
};

#endif /* LevelEvent_hpp */
