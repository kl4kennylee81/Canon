//
//  TutorialStep.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialStep.hpp"
#include "GameState.hpp"

using namespace cugl;

bool TutorialStep::init(std::shared_ptr<TutorialStepData> stepData){
    _currentActiveTime = 0;
    _menu = nullptr;
    _tutStepData = stepData;
    _state = TutorialState::OFF;
    return true;
}

void TutorialStep::update(){
    if (isActive()){
        _currentActiveTime+=GameState::_internalClock->getTimeDilation();
    }
    
    if (_state == TutorialState::POST_ACTIVE && _currentActiveTime >= _tutStepData->getMinTime()){
        _state = TutorialState::DONE;
    }
}
