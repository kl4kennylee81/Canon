//
//  AnimationEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AnimationEvent_hpp
#define AnimationEvent_hpp

#include <stdio.h>
#include "Event.hpp"

class AnimationEvent : public Event {
public:
    enum class AnimationEventType : int {
        /** Signal game is won */
        PLAYER_REMOVED
    };
    
    AnimationEventType _animationType;
    
    bool init() {
        Event::init();
        _eventType = Event::EventType::ANIMATION;
        return true;
    }
    
    static std::shared_ptr<AnimationEvent> alloc() {
        std::shared_ptr<AnimationEvent> result = std::make_shared<AnimationEvent>();
        return (result->init() ? result : nullptr);
    }
    
    AnimationEvent() : Event(){}
    
};

class PlayerRemovedEvent : public AnimationEvent {
public:
    
    bool init() {
        AnimationEvent::init();
        _animationType = AnimationEventType::PLAYER_REMOVED;
        return true;
    }
    
    static std::shared_ptr<PlayerRemovedEvent> alloc() {
        std::shared_ptr<PlayerRemovedEvent> result = std::make_shared<PlayerRemovedEvent>();
        return (result->init() ? result : nullptr);
    }
    
    PlayerRemovedEvent() : AnimationEvent(){}
    
};

#endif /* AnimationEvent_hpp */
