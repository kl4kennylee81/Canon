//
//  GameplayController.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameplayController.hpp"
#include "MenuEvent.hpp"

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

void GameplayController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void GameplayController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void GameplayController::notify(Event* e) {
    BaseController::notify(e);
}
void GameplayController::eventUpdate(Event* e) {
	switch (e->_eventType) {
		case Event::EventType::MENU:
			MenuEvent* menuEvent = static_cast<MenuEvent*>(e);

			switch (menuEvent->_menuEventType) {
				case MenuEvent::MenuEventType::PAUSEGAME:
					PauseGame* pg = static_cast<PauseGame*>(menuEvent);
					if (pg->pause) _paused = true;
					else _paused = false;

			}
			break;
	}
}

void GameplayController::update(float timestep) {
	if (_paused) return;

    // TODO temporary rest until we have a retry screen
    if (_gameState->getReset()){
        // repopulate the randomly generated level
        _levelController->getWorld()->init(_levelController->getWorld()->getAssetManager(),_levelController->getWorld()->getLevelData());
        std::shared_ptr<Scene> s = _gameState->getScene();
        std::shared_ptr<World> w = _levelController->getWorld();
        dispose();
        init(s,w);
        return;
    }
    
    _clockController->update(timestep);
    _levelController->update(timestep, _gameState);
    _spawnController->update(timestep, _gameState);
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

void GameplayController::activate(){
    _gameState->attachToScene();
}

void GameplayController::deactivate(){
    _gameState->detachFromScene();
}


bool GameplayController::init(std::shared_ptr<Scene> scene, std::shared_ptr<World> levelWorld) {
	_gameState = GameState::alloc(scene, levelWorld->getAssetManager());
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

	_pathController->attach(_moveController.get());
    _pathController->attach(_switchController.get());
    _pathController->attach(_animationController.get());
    _pathController->attach(_clockController.get());
    
    _levelController->attach(_collisionController.get());
    _levelController->attach(_animationController.get());
    _levelController->attach(_spawnController.get());
	_levelController->attach(_aiController.get());
    _levelController->attach(_zoneController.get());
    
	_moveController->attach(_switchController.get());
    _moveController->attach(_pathController.get());
    _moveController->attach(_animationController.get());

    _collisionController->attach(_animationController.get());
    _collisionController->attach(_zoneController.get());
    _collisionController->attach(_aiController.get());
    
    _spawnController->attach(_collisionController.get());
    _spawnController->attach(_animationController.get());
    _spawnController->attach(_switchController.get());
    _spawnController->attach(_aiController.get());
    _spawnController->attach(_zoneController.get());
    
    _switchController->attach(_animationController.get());
    
    _zoneController->attach(_collisionController.get());
    _zoneController->attach(_animationController.get());
    
	_paused = false;

    activate();
    
	return true;
}

void GameplayController::dispose(){
    _levelController = nullptr;
    _spawnController = nullptr;
    _switchController = nullptr;
    _pathController = nullptr;
    _moveController = nullptr;
    _aiController = nullptr;
    _collisionController = nullptr;
    _animationController = nullptr;
    _zoneController = nullptr;

    _gameState = nullptr;
}

std::shared_ptr<LevelData> GameplayController::getCurrentLevel(){
    return _levelController->getWorld()->getLevelData();
}

std::shared_ptr<World> GameplayController::getWorld(){
    return _levelController->getWorld();
}
