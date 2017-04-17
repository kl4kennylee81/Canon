//
//  levelEditorEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelEditorEvent_hpp
#define LevelEditorEvent_hpp

#include <stdio.h>
#include "Event.hpp"

class LevelEditorEvent : public Event {
public:
    enum class LevelEditorEventType : int {
        /** Signal that a path is done */
        SIMULATE_LEVEL
    };
    
    LevelEditorEventType _levelEditType;
    
    // potentially include a string to the levelKey to be simulated or the levelData
    
    bool init() {
        Event::init();
        _eventType = Event::EventType::LEVEL_EDITOR;
        return true;
    }
    
    static std::shared_ptr<LevelEditorEvent> alloc() {
        std::shared_ptr<LevelEditorEvent> result = std::make_shared<LevelEditorEvent>();
        return (result->init() ? result : nullptr);
    }
    
    LevelEditorEvent() : Event() {}
};

class SimulateLevelEvent : public LevelEditorEvent {
public:
    
    bool init() {
        LevelEditorEvent::init();
        _levelEditType = LevelEditorEventType::SIMULATE_LEVEL;
        return true;
    }
    
    static std::shared_ptr<SimulateLevelEvent> alloc() {
        std::shared_ptr<SimulateLevelEvent> result = std::make_shared<SimulateLevelEvent>();
        return (result->init() ? result : nullptr);
    }
    
    SimulateLevelEvent() : LevelEditorEvent(){}
};

#endif /* LevelEditorEvent_hpp */
