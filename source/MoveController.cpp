//
//  MoveController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MoveController.hpp"

using namespace cugl;

MoveController::MoveController(){}

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

void MoveController::update(float timestep){}

bool MoveController::init() {
	return true;
}
