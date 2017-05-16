//
//  Event.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class Event {
public:
    /**
     * Enumeration to identify the type of event
     */
    enum class EventType : int {
        /** Signal that a unit died */
        COLLISION,
        /** Signal that a unit spawned */
        LEVEL,
		/** Path **/
		PATH,
		/** Move **/
		MOVE,
        SWITCH,
        BULLET,
        BULLET_SPAWN,
        ZONE,
        ANIMATION,
        /** menu transition system */
        FINISH,
		MENU,
        LEVEL_EDITOR,
        TUTORIAL
    };
    
    EventType _eventType;

	void init() {};

	Event() {};
};

#endif /* Event_hpp */
