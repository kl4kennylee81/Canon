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
#include "GameObject.hpp"

class LevelEvent : public Event {
public:
    enum class LevelEventType : int {
        /** Signal that a path is done */
        OBJECT_SPAWN
    };
    
    LevelEventType levelEventType;
    
    LevelEvent() : Event() {
        _eventType = EventType::LEVEL;
    };
};

class ObjectSpawnEvent : public LevelEvent {
public:
    
    ObjectSpawnEvent(std::shared_ptr<GameObject> object) : LevelEvent() {
        levelEventType = LevelEventType::OBJECT_SPAWN;
        object = object;
    }
    
    std::shared_ptr<GameObject> object;
    
    
};

#endif /* LevelEvent_hpp */
