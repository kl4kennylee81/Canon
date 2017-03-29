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
_clockController(nullptr)
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
    // TODO temporary rest until we have a retry screen
    if (_gameState->reset){
        _levelController->getWorld()->init(_levelController->getWorld()->getAssetManager());
        init(_levelController->getWorld());
        _gameState->reset = false;
        return;
    }
    
    _clockController->update(timestep);
    _levelController->update(timestep, _gameState);
    _spawnController->update(timestep);
    _switchController->update(timestep, _gameState);
    _pathController->update(timestep, _gameState);
    _moveController->update(timestep, _gameState);
    _aiController->update(timestep, _gameState);
    _zoneController->update(timestep);
    _collisionController->update(timestep, _gameState);
    _animationController->update(timestep, _gameState);

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
    _spawnController = SpawnController::alloc();
    _zoneController = ZoneController::alloc(_gameState,levelWorld);
    _animationController = AnimationController::alloc(_gameState,levelWorld->getAssetManager());
	_levelController = LevelController::alloc(_gameState,levelWorld);
    _clockController = ClockController::alloc();

	_pathController->attach(_moveController);
    _pathController->attach(_switchController);
    _pathController->attach(_animationController);
    _pathController->attach(_clockController);
    
    _levelController->attach(_collisionController);
    _levelController->attach(_animationController);
    _levelController->attach(_spawnController);
	_levelController->attach(_aiController);
    _levelController->attach(_zoneController);
    
	_moveController->attach(_switchController);
    _moveController->attach(_pathController);
    _moveController->attach(_animationController);

    _collisionController->attach(_animationController);
    _collisionController->attach(_zoneController);
    
    _spawnController->attach(_collisionController);
    _spawnController->attach(_animationController);
    _spawnController->attach(_switchController);
    _spawnController->attach(_aiController);
    _spawnController->attach(_zoneController);
    
    _switchController->attach(_animationController);
    
    _zoneController->attach(_collisionController);
    _zoneController->attach(_animationController);
    
	return true;
}
