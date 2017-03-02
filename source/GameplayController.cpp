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
_world(nullptr),
_gameState(nullptr),
_pathController(nullptr),
_moveController(nullptr),
_collisionController(nullptr),
_aiController(nullptr),
_switchController(nullptr)
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
    _collisionController->update(timestep, _gameState);
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
    _world = levelWorld;
    _gameState = GameState::alloc(levelWorld->getAssetManager());
    _pathController = PathController::alloc();
    _moveController = MoveController::alloc();
    _collisionController = CollisionController::alloc(_gameState);
    _aiController = AIController::alloc();
    _switchController = SwitchController::alloc();
    return true;
}
