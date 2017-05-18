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
    // self check
    ON_CLICK,
    IMMEDIATE,  // always passes to start
    // path controller
    ON_PATH_DRAWN,
    ON_PATH_START,
    ON_PATH_CANCELED,
    // collision controller
    ON_ENEMY_CLEARED,
    ON_PLAYER_HIT,
    // spawn controller
    ON_ENEMY_SPAWN,
    // level controller
    ON_ENEMY_SPAWNING,
    // move controller
    ON_MOVE_FINISHED,
    NONE
};

enum class TutorialState : int {
    PRE_ACTIVE, // when start condition is met and needs start effects to play
    ACTIVE, // when it's waiting for an end transition condition
    POST_ACTIVE, // when condition is met for end but minimum time is not met
    WAITING, // inactive is when it is waiting for the start transition condition
    OFF, // this step is inactive
    DONE // this step is finished
};

enum class TutorialEffect : int {
    PAUSE_SPAWNING, // pause spawning in levelController
    RESUME_SPAWNING, // resume spawning in levelController
    PAUSE_GAME, // pause every controller but the tutorialController
    RESUME_GAME,
    NONE
};

static TutorialEffect strToTutorialEffect(std::string effect){
    if (effect == "PAUSE_SPAWNING"){
        return TutorialEffect::PAUSE_SPAWNING;
    }
    if (effect == "RESUME_SPAWNING"){
        return TutorialEffect::RESUME_SPAWNING;
    }
    if (effect == "PAUSE_GAME"){
        return TutorialEffect::PAUSE_GAME;
    }
    if (effect == "RESUME_GAME"){
        return TutorialEffect::RESUME_GAME;
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
    if (effect == TutorialEffect::PAUSE_GAME){
        return "PAUSE_GAME";
    }
    if (effect == TutorialEffect::RESUME_GAME){
        return "RESUME_GAME";
    }
    return "";
}

static TutorialTransition strToTransition(std::string trans){
    if (trans == "ON_PATH_CANCELED"){
        return TutorialTransition::ON_PATH_CANCELED;
    }
    if (trans == "ON_MOVE_FINISHED"){
        return TutorialTransition::ON_MOVE_FINISHED;
    }
    if (trans == "ON_PATH_START"){
        return TutorialTransition::ON_PATH_START;
    }
    if (trans == "ON_PLAYER_HIT"){
        return TutorialTransition::ON_PLAYER_HIT;
    }
    if (trans == "ON_ENEMY_SPAWN"){
        return TutorialTransition::ON_ENEMY_SPAWN;
    }
    if (trans == "ON_ENEMY_SPAWNING"){
        return TutorialTransition::ON_ENEMY_SPAWNING;
    }
    if (trans == "ON_CLICK") {
        return TutorialTransition::ON_CLICK;
    }
    if (trans == "ON_PATH_DRAWN") {
        return TutorialTransition::ON_PATH_DRAWN;
    }
    if (trans == "ON_ENEMY_CLEARED") {
        return TutorialTransition::ON_ENEMY_CLEARED;
    }
    if (trans == "IMMEDIATE") {
        return TutorialTransition::IMMEDIATE;
    }
    // default to NONE
    return TutorialTransition::NONE;
}

static std::string transitionToStr(TutorialTransition trans){
    if (trans == TutorialTransition::ON_MOVE_FINISHED){
        return "ON_MOVE_FINISHED";
    }
    if (trans == TutorialTransition::ON_PATH_START) {
        return "ON_PATH_START";
    }
    if (trans == TutorialTransition::ON_PLAYER_HIT){
        return "ON_PLAYER_HIT";
    }
    if (trans == TutorialTransition::ON_ENEMY_SPAWNING){
        return "ON_ENEMY_SPAWNING";
    }
    if (trans == TutorialTransition::ON_ENEMY_SPAWN){
        return "ON_ENEMY_SPAWN";
    }
    if (trans == TutorialTransition::ON_CLICK) {
        return "ON_CLICK";
    }
    if (trans == TutorialTransition::ON_PATH_DRAWN) {
        return "ON_PATH_DRAWN";
    }
    if (trans == TutorialTransition::ON_ENEMY_CLEARED) {
        return "ON_ENEMY_CLEARED";
    }
    if (trans == TutorialTransition::IMMEDIATE) {
        return "IMMEDIATE";
    }
    if (trans == TutorialTransition::ON_PATH_CANCEL){
        return "ON_PATH_CANCELED";
    }
    return "";
}

#endif /* TutorialEnums_hpp */
