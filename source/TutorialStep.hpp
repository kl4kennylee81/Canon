//
//  TutorialStep.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialStep_hpp
#define TutorialStep_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "UIComponent.hpp"

enum class TutorialTransition : int {
    ON_CLICK,
    ON_PATH_DRAWN,
    ON_ENEMY_CLEARED,
    NONE
};

enum class TutorialState : int {
    ACTIVE, // when it's waiting for an end transition condition
    WAITING, // inactive is when it is waiting for the start transition condition
    OFF, // this step is inactive
    DONE // this step is finished
};

class TutorialStep {
private:
    TutorialTransition _startCondition;
    TutorialTransition _endCondition;
    std::shared_ptr<UIComponent> _uiComponent;
    TutorialState _state;
    
public:
    TutorialStep():
    _uiComponent(nullptr),
    _startCondition(TutorialTransition::NONE),
    _endCondition(TutorialTransition::NONE),
    _state(TutorialState::OFF)
    {};
    
    ~TutorialStep(){ dispose(); };
    
    bool init();
    
    void dispose(){
        _uiComponent = nullptr;
        _startCondition = TutorialTransition::NONE;
        _endCondition = TutorialTransition::NONE;
        _state = TutorialState::OFF;
    }
    
    void setUIComponent(std::shared_ptr<UIComponent> ui) {
        _uiComponent = ui;
    }
    
    void setTransition(TutorialTransition start, TutorialTransition end){
        _startCondition = start;
        _endCondition = end;
    }
    
    static std::shared_ptr<TutorialStep> alloc() {
        std::shared_ptr<TutorialStep> result = std::make_shared<TutorialStep>();
        return (result->init() ? result : nullptr);
    };
    
    TutorialTransition getStartCondition(){
        return _startCondition;
    }
    
    TutorialTransition getEndCondition(){
        return _endCondition;
    }
    
    std::shared_ptr<UIComponent> getUIComponent(){
        return _uiComponent;
    }
    
    TutorialState getState(){
        return _state;
    }
    
    bool isDone(){
        return _state == TutorialState::DONE;
    }
    
    void setState(TutorialState state){
        _state = state;
    }
    
    void checkCondition(TutorialTransition transition){
        if (_state == TutorialState::WAITING && _startCondition == transition){
            _state = TutorialState::ACTIVE;
            return;
        }
        
        if (_state == TutorialState::ACTIVE && _endCondition == transition){
            _state = TutorialState::DONE;
            return;
        }
        return;
    }
};

#endif /* TutorialStep_hpp */
