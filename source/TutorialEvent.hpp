//
//  TutorialEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/15/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialEvent_hpp
#define TutorialEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"

class TutorialEvent : public Event {
public:
    enum class TutorialEventType : int {
        PAUSE_SPAWNING,
        RESUME_SPAWNING
    };
    
    TutorialEventType _TutorialType;
    
    bool init() {
        Event::init();
        _eventType = Event::EventType::TUTORIAL;
        return true;
    }
    
    static std::shared_ptr<TutorialEvent> alloc() {
        std::shared_ptr<TutorialEvent> result = std::make_shared<TutorialEvent>();
        return (result->init() ? result : nullptr);
    }
    
    TutorialEvent() : Event(){}
    
};

class PauseSpawningEvent : public TutorialEvent {
public:
    
    bool init() {
        TutorialEvent::init();
        _TutorialType = TutorialEventType::PAUSE_SPAWNING;
        return true;
    }
    
    static std::shared_ptr<PauseSpawningEvent> alloc() {
        std::shared_ptr<PauseSpawningEvent> result = std::make_shared<PauseSpawningEvent>();
        return (result->init() ? result : nullptr);
    }
    
    PauseSpawningEvent() : TutorialEvent(){}
    
};

class ResumeSpawningEvent : public TutorialEvent {
public:
    
    bool init() {
        TutorialEvent::init();
        _TutorialType = TutorialEventType::RESUME_SPAWNING;
        return true;
    }
    
    static std::shared_ptr<ResumeSpawningEvent> alloc() {
        std::shared_ptr<ResumeSpawningEvent> result = std::make_shared<ResumeSpawningEvent>();
        return (result->init() ? result : nullptr);
    }
    
    ResumeSpawningEvent() : TutorialEvent(){}
    
};

#endif /* TutorialEvent_hpp */
