//
//  PathController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PathController.hpp"

using namespace cugl;

PathController::PathController():
BaseController(){}

void PathController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}

void PathController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void PathController::notify(Event* e) {
	BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void PathController::eventUpdate(Event* e) {}

void PathController::update(float timestep,std::shared_ptr<GameState> state)
{}

bool PathController::init() {
	return true;
}
