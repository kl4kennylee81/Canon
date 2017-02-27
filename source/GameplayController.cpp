//
//  GameplayController.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameplayController.hpp"

using namespace cugl;

void GameplayController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}
void GameplayController::detach(std::shared_ptr<Observer> obs) {
	BaseController::detach(obs);
}
void GameplayController::notify(Event* e) {
	BaseController::notify(e);
}
void GameplayController::eventUpdate(Event* e) {}

bool GameplayController::init() {
	_world = World::alloc();
	_gameState = GameState::alloc();
	_pathController = PathController::alloc();
	_moveController = MoveController::alloc();
	_collisionController = CollisionController::alloc();
	_aiController = AIController::alloc();
	return true;
}
