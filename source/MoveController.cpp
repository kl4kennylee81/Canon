//
//  MoveController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MoveController.hpp"

using namespace cugl;

MoveController::MoveController():
BaseController(){}

void MoveController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}

void MoveController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void MoveController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void MoveController::eventUpdate(Event* e) {}

void MoveController::update(float timestep,std::shared_ptr<GameState> state){}

bool MoveController::init() {
	return true;
}
