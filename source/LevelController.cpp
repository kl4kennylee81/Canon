//
//  LevelController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelController.hpp"

using namespace cugl;

LevelController::LevelController():
BaseController(),
_world(nullptr){}

void LevelController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void LevelController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void LevelController::notify(Event* e) {
    BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void LevelController::eventUpdate(Event* e) {}

void LevelController::update(float timestep,std::shared_ptr<GameState> state){
    int waveKey = _level.pollWave();
    if (waveKey != -1){
        // spawn the gameObject from the prototypes
        
    }
    _level.update(timestep);
}

bool LevelController::init(std::shared_ptr<World> world) {
    _world = world;
    _level.init(world->getLevelData());
    return true;
}
