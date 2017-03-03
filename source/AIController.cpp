//
//  AIController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AIController.hpp"
#include "MoveController.hpp"
#include "Element.hpp"
#include "PathParameters.h"

using namespace cugl;

AIController::AIController():
BaseController(){}

void AIController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}
void AIController::detach(Observer* obs) {
	BaseController::detach(obs);
}
void AIController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void AIController::eventUpdate(Event* e) {}

void AIController::update(float timestep,std::shared_ptr<GameState> state){
	for (auto it : state->getEnemyObjects()) {
		Element e = it->getPhysicsComponent()->getElementType();
		int playerIndex = e == Element::BLUE ? 1 : 0;
		auto player = state->getPlayerCharacters().at(playerIndex);
		Vec2 playerPos = player->getNode()->getPosition();
		Vec2 enemyPos = it->getNode()->getPosition();
		Vec2 direction = MoveController::getVelocityVector(enemyPos, playerPos, AI_SPEED);
		it->getPhysicsComponent()->getBody()->setLinearVelocity(direction);
	}
}

bool AIController::init() {
	return true;
}
