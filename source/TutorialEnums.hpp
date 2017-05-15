//
//  TutorialEnums.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/14/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialEnums_hpp
#define TutorialEnums_hpp

#include <stdio.h>
#include <cugl/cugl.h>

enum class TutorialTransition : int {
    ON_CLICK,
    ON_PATH_DRAWN,
    ON_ENEMY_CLEARED,
    ON_START,  // at the beginning of init when gien a tutorialLevel will go to active
    NONE
};

enum class TutorialState : int {
    ACTIVE, // when it's waiting for an end transition condition
    WAITING, // inactive is when it is waiting for the start transition condition
    OFF, // this step is inactive
    DONE // this step is finished
};

enum class TutorialEffect : int {
    PAUSE_SPAWNING, // pause spawning in levelController
    RESUME_SPAWNING, // resume spawning in levelController
    NONE
};

static TutorialEffect strToTutorialEffect(std::string effect){
    if (effect == "PAUSE_SPAWNING"){
        return TutorialEffect::PAUSE_SPAWNING;
    }
    if (effect == "RESUME_SPAWNING"){
        return TutorialEffect::RESUME_SPAWNING;
    }
    return TutorialEffect::NONE;
}

static std::string tutorialEffectToStr (TutorialEffect effect){
    if (effect == TutorialEffect::PAUSE_SPAWNING){
        return "PAUSE_SPAWNING";
    }
    if (effect == TutorialEffect::RESUME_SPAWNING){
        return "RESUME_SPAWNING";
    }
    return "";
}

static TutorialTransition strToTransition(std::string trans){
    if (trans == "ON_CLICK") {
        return TutorialTransition::ON_CLICK;
    }
    if (trans == "ON_PATH_DRAWN") {
        return TutorialTransition::ON_PATH_DRAWN;
    }
    if (trans == "ON_ENEMY_CLEARED") {
        return TutorialTransition::ON_ENEMY_CLEARED;
    }
    if (trans == "ON_START") {
        return TutorialTransition::ON_START;
    }
    // default to NONE
    return TutorialTransition::NONE;
}

static std::string transitionToStr(TutorialTransition trans){
    if (trans == TutorialTransition::ON_CLICK) {
        return "ON_CLICK";
    }
    if (trans == TutorialTransition::ON_PATH_DRAWN) {
        return "ON_PATH_DRAWN";
    }
    if (trans == TutorialTransition::ON_ENEMY_CLEARED) {
        return "ON_ENEMY_CLEARED";
    }
    if (trans == TutorialTransition::ON_START) {
        return "ON_START";
    }
    return "";
}

#endif /* TutorialEnums_hpp */
