//
//  SyncController.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/2/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SyncController.hpp"

using namespace cugl;

SyncController::SyncController():
BaseController(){}

void SyncController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void SyncController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void SyncController::notify(Event* e) {
    BaseController::notify(e);
}
void SyncController::eventUpdate(Event* e) {}

void SyncController::update(float timestep,std::shared_ptr<GameState> state) {
    for(auto it = state->getPlayerCharacters().begin() ; it != state->getPlayerCharacters().end(); ++it) {
        it->get()->sync(GameState::_physicsScale);
    }

    for(auto it = state->getEnemyObjects().begin() ; it != state->getEnemyObjects().end(); ++it) {
        it->get()->sync(GameState::_physicsScale);
    }
}

bool SyncController::init() {
    return true;
}
