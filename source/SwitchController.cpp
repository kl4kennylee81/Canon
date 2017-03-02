//
//  SwitchController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SwitchController.hpp"

using namespace cugl;

SwitchController::SwitchController():
BaseController(){}

void SwitchController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void SwitchController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void SwitchController::notify(Event* e) {
    BaseController::notify(e);
}
void SwitchController::eventUpdate(Event* e) {}

void SwitchController::update(float timestep,std::shared_ptr<GameState> state) {
    
}

bool SwitchController::init() {
    return true;
}
