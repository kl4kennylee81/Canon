//
//  ActiveAnimation.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveAnimation.hpp"

void ActiveAnimation::handleEvent(AnimationEvent event) {
    if (!eventExists(event)) {
        return;
    }
    std::shared_ptr<AnimationUpdate> upd = getAnimationUpdate(event);
    active = upd->active;
    if (!upd->repeat.empty()){
        repeat = upd->repeat;
    }
    curFrames = 0;
    curIndex = 0;
}

bool ActiveAnimation::nextFrame() {
    curFrames++;
    std::shared_ptr<AnimationState> animState = getAnimationState();
    int totNumFrames = animState->frames.at(curIndex);
    if (curFrames == totNumFrames){
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
