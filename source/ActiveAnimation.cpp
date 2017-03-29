//
//  ActiveAnimation.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveAnimation.hpp"

void ActiveAnimation::handleAction(AnimationAction action) {
    if (!eventExists(action)) {
        return;
    }
    std::shared_ptr<AnimationUpdate> upd = getAnimationUpdate(action);
    active = upd->active;
    if (!upd->repeat.empty()){
        repeat = upd->repeat;
    }
    curFrames = 0;
    curIndex = 0;
}

bool ActiveAnimation::nextFrame() {
    curFrames += GameState::_internalClock->getTimeDilation();
    
    std::shared_ptr<AnimationState> animState = getAnimationState();
    int totNumFrames = animState->frames.at(curIndex);
    if (curFrames >= totNumFrames){
        curFrames = 0;
        curIndex++;
    }
    if (curIndex == animState->total){
        if (_last){
            return false;
        }
        if (!active.empty()){
            active = std::string();
        }
        curIndex = 0;
    }
    _node->setFrame(getAnimationState()->first+curIndex);
    return true;
}

bool ActiveAnimation::isUniformScaling(){
    return !this->_data->nonUniformScale;
}
