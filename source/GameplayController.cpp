//
//  GameplayController.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameplayController.hpp"

using namespace cugl;

GameplayController::GameplayController() :
BaseController(),
_gameState(nullptr),
_pathController(nullptr),
_moveController(nullptr),
_collisionController(nullptr),
_aiController(nullptr),
_switchController(nullptr),
_levelController(nullptr),
_syncController(nullptr)
{}

void GameplayController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void GameplayController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void GameplayController::notify(Event* e) {
    BaseController::notify(e);
}
void GameplayController::eventUpdate(Event* e) {}

void GameplayController::update(float timestep) {
	if (_gameState->reset) {
        std::shared_ptr<World> newWorld = World::alloc(_levelController->getWorld()->getAssetManager());
        init(newWorld);
	}
	else {
		_levelController->update(timestep, _gameState);
		_pathController->update(timestep, _gameState);
		_moveController->update(timestep, _gameState);
		_collisionController->update(timestep, _gameState);
		_syncController->update(timestep, _gameState);
		_moveController->updateActivePaths(timestep, _gameState);
		_aiController->update(timestep, _gameState);
		_switchController->update(timestep, _gameState);
		_animationController->update(timestep, _gameState);
	}
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void GameplayController::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    _gameState->draw(_batch);
}


bool GameplayController::init(std::shared_ptr<World> levelWorld) {
	_gameState = GameState::alloc(levelWorld->getAssetManager());
	_pathController = PathController::alloc(_gameState);
	_moveController = MoveController::alloc(_gameState);
	_collisionController = CollisionController::alloc(_gameState);
	_aiController = AIController::alloc();
    _switchController = SwitchController::alloc(_gameState);
	_levelController = LevelController::alloc(levelWorld);
	_syncController = SyncController::alloc();
    _animationController = AnimationController::alloc(_gameState);

	_pathController->attach(_moveController);
    _pathController->attach(_switchController);
    
    _levelController->attach(_collisionController);
    _levelController->attach(_animationController);

	_moveController->attach(_switchController);

//	_collisionController->attach(_moveController);
    _collisionController->attach(_animationController);

	return true;
}
