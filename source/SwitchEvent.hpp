//
//  PathEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SwitchEvent_hpp
#define SwitchEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/base/CUBase.h>
#include "Event.hpp"
#include "GameEngine.hpp"

class SwitchEvent : public Event {
public:
    enum class SwitchEventType : int {
        /** Signal that a path is done */
        ACTIVE_CHARACTER,
    };
    
    std::shared_ptr<GameObject> _character;
    
    bool init( std::shared_ptr<GameObject> c) {
        Event::init();
        _eventType = Event::EventType::SWITCH;
        _character = c;
        return true;
    }
    
    static std::shared_ptr<SwitchEvent> alloc( std::shared_ptr<GameObject> c) {
        std::shared_ptr<SwitchEvent> result = std::make_shared<SwitchEvent>();
        return (result->init(c) ? result : nullptr);
    }
    
    SwitchEvent() : Event() {}
};


#endif /* SwitchEvent_hpp */
