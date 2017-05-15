//
//  TutorialStep.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialStep.hpp"

bool TutorialStep::init(std::shared_ptr<TutorialStepData> stepData){
    _menu = nullptr;
    _tutStepData = stepData;
    _state = TutorialState::OFF;
    return true;
}
