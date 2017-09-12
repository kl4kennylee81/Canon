//
//  FinishEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef FinishEvent_hpp
#define FinishEvent_hpp

#include <stdio.h>
#include "Event.hpp"

class FinishEvent : public Event {
public:
    enum class FinishEventType : int {
        // TODO: it's more accurate to have it be level won, level lost, and game won
        /** Signal game is won */
        GAME_WON,
        /** Signal game is lost */
        GAME_LOST,
        /** Signal that last level of game is beaten **/
        GAME_COMPLETED
    };
    
    FinishEventType _finishType;
    
    bool init() {
        Event::init();
        _eventType = Event::EventType::FINISH;
        return true;
    }
    
    static std::shared_ptr<FinishEvent> alloc() {
        std::shared_ptr<FinishEvent> result = std::make_shared<FinishEvent>();
        return (result->init() ? result : nullptr);
    }
    
    FinishEvent() : Event(){}
    
};

class GameLostEvent : public FinishEvent {
public:
    
    bool init() {
        FinishEvent::init();
        _finishType = FinishEventType::GAME_LOST;
        return true;
    }
    
    static std::shared_ptr<GameLostEvent> alloc() {
        std::shared_ptr<GameLostEvent> result = std::make_shared<GameLostEvent>();
        return (result->init() ? result : nullptr);
    }
    
    GameLostEvent() : FinishEvent(){}
    
};

class GameWonEvent : public FinishEvent {
public:
    bool init() {
        FinishEvent::init();
        _finishType = FinishEventType::GAME_WON;
        return true;
    }
    
    static std::shared_ptr<GameWonEvent> alloc() {
        std::shared_ptr<GameWonEvent> result = std::make_shared<GameWonEvent>();
        return (result->init() ? result : nullptr);
    }
    
    GameWonEvent() : FinishEvent(){}
};

class GameCompletedEvent : public FinishEvent {
public:
    bool init() {
        FinishEvent::init();
        _finishType = FinishEventType::GAME_COMPLETED;
        return true;
    }
    
    static std::shared_ptr<GameCompletedEvent> alloc() {
        std::shared_ptr<GameCompletedEvent> result = std::make_shared<GameCompletedEvent>();
        return (result->init() ? result : nullptr);
    }
    
    GameCompletedEvent() : FinishEvent(){}
    
};

#endif /* FinishEvent_hpp */
