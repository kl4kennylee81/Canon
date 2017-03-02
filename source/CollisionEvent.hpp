//
//  CollisionEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef CollisionEvent_hpp
#define CollisionEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"
#include "GameObject.hpp"

class CollisionEvent : public Event {
public:
    enum class CollisionEventType : int {
        /** Signal that an object is gone */
        OBJECT_GONE,
        /** Signal that an object is hit */
        OBJECT_HIT,
        /** Signal game is over */
        GAME_LOST
    };

    CollisionEventType _collisionType;
    
    bool init() {
        Event::init();
        _eventType = Event::EventType::COLLISION;
        return true;
    }
    
    static std::shared_ptr<CollisionEvent> alloc() {
        std::shared_ptr<CollisionEvent> result = std::make_shared<CollisionEvent>();
        return (result->init() ? result : nullptr);
    }
    
    CollisionEvent() : Event(){}
    
};

class ObjectGoneEvent : public CollisionEvent {
public:
    GameObject* _obj;
    
    bool init(GameObject* obj) {
        CollisionEvent::init();
        _obj = obj;
        _collisionType = CollisionEventType::OBJECT_GONE;
        return true;
    }
    
    static std::shared_ptr<ObjectGoneEvent> alloc(GameObject* obj) {
        std::shared_ptr<ObjectGoneEvent> result = std::make_shared<ObjectGoneEvent>();
        return (result->init(obj) ? result : nullptr);
    }
    
    ObjectGoneEvent() : CollisionEvent(){}
    
};

class ObjectHitEvent : CollisionEvent {

};

class GameLostEvent : CollisionEvent {

};

#endif /* CollisionEvent_hpp */
