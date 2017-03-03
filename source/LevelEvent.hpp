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
        this->_eventType = EventType::LEVEL;
    };
};

class ObjectSpawnEvent : public LevelEvent {
public:
    
    std::shared_ptr<GameObject> object;
    
    ObjectSpawnEvent() : LevelEvent() {
        levelEventType = LevelEventType::OBJECT_SPAWN;
    }
    
    bool init(std::shared_ptr<GameObject> object){
        this->levelEventType = LevelEventType::OBJECT_SPAWN;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ObjectSpawnEvent> alloc(std::shared_ptr<GameObject> object){
        std::shared_ptr<ObjectSpawnEvent> result = std::make_shared<ObjectSpawnEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

#endif /* LevelEvent_hpp */
