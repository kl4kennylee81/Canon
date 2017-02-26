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

class CollisionEvent : Event {
    enum class CollisionEventType : int {
        /** Signal that an object is gone */
        OBJECT_GONE,
        /** Signal that an object is hit */
        OBJECT_HIT,
        /** Signal game is over */
        GAME_LOST
    };

    CollisionEventType collisionEventType;
    
    GameObject* obj1;
    
    GameObject* obj2;
    
};

class ObjectGoneEvent : CollisionEvent {
    
};

class ObjectHitEvent : CollisionEvent {

};

class GameLostEvent : CollisionEvent {

};

#endif /* CollisionEvent_hpp */
