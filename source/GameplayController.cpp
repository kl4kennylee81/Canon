//
//  GameplayController.cpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameplayController.hpp"
#include "MenuEvent.hpp"
#include "FinishEvent.hpp"

using namespace cugl;

#define RUSTLING_LEAVES "./sounds/rustlingleaves.mp3"

GameplayController::GameplayController() :
BaseController(),
_gameState(nullptr),
_pathController(nullptr),
_moveController(nullptr),
_collisionController(nullptr),
_aiController(nullptr),
_bulletController(nullptr),
_switchController(nullptr),
_levelController(nullptr),
_clockController(nullptr),
_finishController(nullptr),
_soundController(nullptr),
_tutorialController(nullptr)
{}

GameplayController::~GameplayController(){
    dispose();
}

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
        {
			MenuEvent* menuEvent = static_cast<MenuEvent*>(e);

			switch (menuEvent->_menuEventType) {
				case MenuEvent::MenuEventType::PAUSEGAME:
                {
					PauseGameEvent* pg = static_cast<PauseGameEvent*>(menuEvent);
                    _paused = pg->pause;
                    break;
                }
                case MenuEvent::MenuEventType::RESETGAME:
                {
                    _gameState->toggleReset();
                    break;
                }

			}
			break;
        }
        case Event::EventType::FINISH:
        {
            FinishEvent* finishEvent = static_cast<FinishEvent*>(e);
            switch(finishEvent->_finishType){
                case FinishEvent::FinishEventType::GAME_WON:
                {
                    // route it onward to the observers of the gameplay controller
                    // which is the menu controller
                    this->notify(e);
                    _paused = true;
                    break;
                }
                case FinishEvent::FinishEventType::GAME_LOST:
                {
                    // route it onward to the observers of the gameplay controller
                    // which is the menu controller
                    this->notify(e);
                    _paused = true;
                    break;
                }
            }
            break;
        }
	}
}

void GameplayController::update(float timestep) {

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
    // TODO hacky way to pause the game
	if (_paused) return;
    
    _clockController->update(timestep);
    _levelController->update(timestep, _gameState);
    _spawnController->update(timestep, _gameState);
    _switchController->update(timestep, _gameState);
    _pathController->update(timestep, _gameState);
    _moveController->update(timestep, _gameState);
    _aiController->update(timestep, _gameState);
    _bulletController->update(timestep, _gameState);
    _zoneController->update(timestep);
    _collisionController->update(timestep, _gameState);
    _animationController->update(timestep, _gameState);
    _particleController->update(timestep, _gameState);
    _soundController->update(timestep, _gameState);
    _tutorialController->update(timestep, _gameState);
    _finishController->update(timestep, _gameState);
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
	_pathController = PathController::alloc(_gameState, levelWorld);
	_moveController = MoveController::alloc(_gameState);
	_collisionController = CollisionController::alloc(_gameState);
	_aiController = AIController::alloc();
    _bulletController = BulletController::alloc();
    _switchController = SwitchController::alloc(_gameState);
    _spawnController = SpawnController::alloc();
    _zoneController = ZoneController::alloc(_gameState,levelWorld);
    _animationController = AnimationController::alloc(_gameState,levelWorld->getAssetManager());
	_levelController = LevelController::alloc(_gameState,levelWorld);
    _clockController = ClockController::alloc();
    _particleController = ParticleController::alloc(_gameState, levelWorld->getAssetManager());
    _finishController = FinishController::alloc();
    _soundController = SoundController::alloc(levelWorld);
    _tutorialController = TutorialController::alloc(_gameState);

	_pathController->attach(_moveController.get());
    _pathController->attach(_switchController.get());
    _pathController->attach(_animationController.get());
    _pathController->attach(_clockController.get());
    _pathController->attach(_soundController.get());
    _pathController->attach(_zoneController.get());
    
    _levelController->attach(_collisionController.get());
    _levelController->attach(_animationController.get());
    _levelController->attach(_spawnController.get());
	_levelController->attach(_aiController.get());
    _levelController->attach(_bulletController.get());
    _levelController->attach(_zoneController.get());
    _levelController->attach(_finishController.get());
    _levelController->attach(_soundController.get());
    _levelController->attach(_particleController.get());
    
	_moveController->attach(_switchController.get());
    _moveController->attach(_pathController.get());
    _moveController->attach(_animationController.get());

    _collisionController->attach(_animationController.get());
    _collisionController->attach(_zoneController.get());
    _collisionController->attach(_aiController.get());
    _collisionController->attach(_bulletController.get());
    _collisionController->attach(_soundController.get());
    _collisionController->attach(_particleController.get());
    
    _spawnController->attach(_collisionController.get());
    _spawnController->attach(_animationController.get());
    _spawnController->attach(_switchController.get());
    _spawnController->attach(_bulletController.get());
    _spawnController->attach(_aiController.get());
    _spawnController->attach(_zoneController.get());
    _spawnController->attach(_soundController.get());
    _spawnController->attach(_pathController.get());
    
    _bulletController->attach(_levelController.get());
    
    _switchController->attach(_animationController.get());
    
    _zoneController->attach(_collisionController.get());
    _zoneController->attach(_animationController.get());
    _zoneController->attach(_particleController.get());
    
    _animationController->attach(_finishController.get());
    
    // attach the gameplayController to the finishController
    _finishController->attach(this);
    
	_paused = false;

    activate();
    
    AudioEngine::get()->playMusic(levelWorld->getAssetManager()->get<Music>("rustling_leaves"),true);
    
	return true;
}

void GameplayController::dispose(){
    _levelController = nullptr;
    _spawnController = nullptr;
    _switchController = nullptr;
    _pathController = nullptr;
    _moveController = nullptr;
    _aiController = nullptr;
    _bulletController = nullptr;
    _collisionController = nullptr;
    _animationController = nullptr;
    _zoneController = nullptr;
    _soundController = nullptr;

    _gameState = nullptr;
}

std::shared_ptr<LevelData> GameplayController::getCurrentLevel(){
    return _levelController->getWorld()->getLevelData();
}

std::shared_ptr<World> GameplayController::getWorld(){
    return _levelController->getWorld();
}

std::shared_ptr<cugl::JsonValue> GameplayController::toJsonValue(){
    // TODO kelly
    // call the toJsonValue for eachController that needs serialization
    // and collect into one big gameplayController json and return it
    
    std::shared_ptr<JsonValue> gameplayJson = JsonValue::allocObject();
    gameplayJson->appendChild("levelController", _levelController->toJsonValue(_gameState));
    gameplayJson->appendChild("spawnController", _spawnController->toJsonValue());
	gameplayJson->appendChild("moveController", _moveController->toJsonValue());
	gameplayJson->appendChild("aiController", _aiController->toJsonValue());
    return gameplayJson;
}

void GameplayController::onResume(const std::shared_ptr<cugl::JsonValue> resumeJson){
    // give level controller the full json since it uses spawn and level
    _levelController->initAfterResume(_gameState,resumeJson->get("levelController"), resumeJson->get("spawnController"));
	_moveController->initAfterResume(_gameState, resumeJson->get("moveController"));
	_aiController->initAfterResume(_gameState, resumeJson->get("aiController"));
}
