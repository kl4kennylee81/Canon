//
//  GameState.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameState.hpp"

std::unique_ptr<InternalClock> GameState::_internalClock(new InternalClock());

using namespace cugl;

/** The name of the space texture */
// HACK replace with level loading sending event
#define BACKGROUND_TEXTURE       "bg_dark_lake"
#define NUM_PLAYER_CHARS         2

bool GameState::init(std::shared_ptr<Scene> scene, const std::shared_ptr<GenericAssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    // reinitialize the static clock
    GameState::_internalClock->init();
    _reset = false;
    _activeCharacterPosition = 0;
    
    Rect size = scene->getCamera()->getViewport();
    
    // magic numbers are okay as long as 16:9
    _bounds = Rect::Rect(0,0,GAME_PHYSICS_WIDTH,GAME_PHYSICS_HEIGHT);
    
    float world_yPos = (size.getMaxY() - (_bounds.getMaxY() * GAME_PHYSICS_SCALE))/2;
    Vec2 world_pos = Vec2::Vec2(0,world_yPos);
    
    // Get the space background.  Its size determines all scaling.
    auto image = assets->get<Texture>(BACKGROUND_TEXTURE);
    auto bkgdTextureNode = PolygonNode::allocWithTexture(image);
    bkgdTextureNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgdTextureNode->setPosition(Vec2::ZERO);
    
    _bgnode = Node::alloc();
    _bgnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgnode->setPosition(world_pos);
    _bgnode->addChild(bkgdTextureNode);
    
    _worldnode = Node::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setScale(GAME_PHYSICS_SCALE);
    _worldnode->setPosition(world_pos);
    
    _debugnode = Node::alloc();
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setScale(GAME_PHYSICS_SCALE);
    _debugnode->setPosition(world_pos);
    
    _scene = scene;
    _gameplayNode = Node::alloc();
    
    // we don't attach to scene directly have the game engine handle when to attach
    _gameplayNode->addChild(_bgnode,0);
    _gameplayNode->addChild(_worldnode,1);
    _gameplayNode->addChild(_debugnode,2);
    
    return true;
}

void GameState::dispose(){
    _reset = false;
    _scene = nullptr;
    _gameplayNode = nullptr;
    _worldnode = nullptr;
    _debugnode = nullptr;
    _bgnode = nullptr;
    _bounds = Rect();
    _activeCharacterPosition = 0;
}

void GameState::attachToScene(){
    _scene->addChild(_gameplayNode);
}

void GameState::detachFromScene(){
    _scene->removeChild(_gameplayNode);
}

std::shared_ptr<GameObject> GameState::getActiveCharacter()
{
    if (_playerCharacters.size() != NUM_PLAYER_CHARS){
        return nullptr;
    }
    return _playerCharacters.at(_activeCharacterPosition);
}

bool GameState::getReset(){
    return _reset;
}

void GameState::toggleReset(){
    _reset = !_reset;
}

/**
 * The method called to draw the gameState to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void GameState::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    _scene->render(_batch);
}


void GameState::addEnemyGameObject(std::shared_ptr<GameObject> obj){
    _enemyObjects.push_back(obj);
}

void GameState::addPlayerGameObject(std::shared_ptr<GameObject> obj){
    _playerCharacters.push_back(obj);
}

std::shared_ptr<GameObject> GameState::getPlayer(int index){
    if (_playerCharacters.size() <= index){
        return nullptr;
    }
    return _playerCharacters.at(index);
}

size_t GameState::getNumberPlayerCharacters(){
    return _playerCharacters.size();
}

#pragma mark Coordinate Conversions

float GameState::getPhysicsScale(){
    return GAME_PHYSICS_SCALE;
}




