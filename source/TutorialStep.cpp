//
//  TutorialStep.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialStep.hpp"

bool TutorialStep::init(){
    _menu = nullptr;
    _startCondition = TutorialTransition::NONE;
    _endCondition = TutorialTransition::NONE;
    _state = TutorialState::OFF;
    return true;
}
