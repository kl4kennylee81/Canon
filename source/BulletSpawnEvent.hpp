//
//  BulletSpawnEvent.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletSpawnEvent_hpp
#define BulletSpawnEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"
#include "GameObject.hpp"

class BulletSpawnEvent : public Event {
public:
    std::shared_ptr<GameObject> object;
    float velocity;
    float angle;
    cugl::Vec2 position;
    
    BulletSpawnEvent() : Event() {
        this->_eventType = EventType::BULLET_SPAWN;
    }
    
    bool init(std::shared_ptr<GameObject> obj, float vel, float ang){
        object = obj;
        angle = ang;
        velocity = vel;
        return true;
    }
    
    static std::shared_ptr<BulletSpawnEvent> alloc(std::shared_ptr<GameObject> obj, float vel, float ang){
        std::shared_ptr<BulletSpawnEvent> result = std::make_shared<BulletSpawnEvent>();
        return (result->init(obj, vel, ang) ? result : nullptr);
    }
};
#endif /* BulletSpawnEvent_hpp */
