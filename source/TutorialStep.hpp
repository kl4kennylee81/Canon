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
#include "Menu.hpp"
#include "TutorialEnums.hpp"
#include "TutorialStepData.hpp"

class TutorialStep {
private:
    std::shared_ptr<TutorialStepData> _tutStepData;
    std::shared_ptr<Menu> _menu;
    TutorialState _state;
    
public:
    TutorialStep():
    _menu(nullptr),
    _tutStepData(nullptr),
    _state(TutorialState::OFF)
    {};
    
    ~TutorialStep(){ dispose(); };
    
    bool init(std::shared_ptr<TutorialStepData> stepData);
    
    void dispose(){
        _menu = nullptr;
        _tutStepData = nullptr;
        _state = TutorialState::OFF;
    }
    
    void setMenu(std::shared_ptr<Menu> menu) {
        _menu = menu;
    }
    
    static std::shared_ptr<TutorialStep> alloc(std::shared_ptr<TutorialStepData> stepData) {
        std::shared_ptr<TutorialStep> result = std::make_shared<TutorialStep>();
        return (result->init(stepData) ? result : nullptr);
    };
    
    TutorialTransition getStartCondition(){
        return _tutStepData->getStartCondition();
    }
    
    TutorialTransition getEndCondition(){
        return _tutStepData->getEndCondition();
    }
    
    std::shared_ptr<Menu> getMenu(){
        return _menu;
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
        if (_tutStepData == nullptr){
            return;
        }
        
        if (_state == TutorialState::WAITING && getStartCondition() == transition){
            _state = TutorialState::PRE_ACTIVE;
            return;
        }
        
        if (_state == TutorialState::ACTIVE && getEndCondition() == transition){
            _state = TutorialState::DONE;
            return;
        }
        return;
    }
    
    void addToNode(std::shared_ptr<cugl::Node> node){
        _menu->attachToScene(node);
    }
    
    const std::shared_ptr<TutorialStepData> getStepData(){
        return _tutStepData;
    }
    
    bool isActive(){
        return _state == TutorialState::POST_ACTIVE || _state == TutorialState::ACTIVE;
    }
};

#endif /* TutorialStep_hpp */
