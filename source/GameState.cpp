//
//  GameState.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameState.hpp"
#include "Util.hpp"

std::unique_ptr<InternalClock> GameState::_internalClock(new InternalClock());

using namespace cugl;

/** The name of the space texture */
// HACK replace with level loading sending event

#define BACKGROUND_TEXTURE       "bg_blue_sky"
#define NUM_PLAYER_CHARS         2

bool GameState::init(std::shared_ptr<Scene> scene, const std::shared_ptr<GenericAssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    // reinitialize the static clock
    GameState::_internalClock->init();
    
    // reset the gameObjects atomic counter
    GameObject::resetAtomicUidCounter();
    
    _reset = false;
    _activeCharacterPosition = 0;
    
    Rect size = scene->getCamera()->getViewport();
    
    // magic numbers are okay as long as 16:9
    _bounds = Rect::Rect(0,0,GAME_PHYSICS_WIDTH,GAME_PHYSICS_HEIGHT);
    
    float world_yPos = (size.getMaxY() - (_bounds.getMaxY() * GAME_PHYSICS_SCALE))/2;
    Vec2 world_pos = Vec2::Vec2(0,world_yPos);
    
    // set the bkgd texture in the levelData
    auto image = assets->get<Texture>(BACKGROUND_TEXTURE);
    auto bkgdTextureNode = PolygonNode::allocWithTexture(image);
    bkgdTextureNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgdTextureNode->setPosition(Vec2::ZERO);
    bkgdTextureNode->setScale(((float)GAME_SCENE_WIDTH)/image->getWidth());
    
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
    if (_scene->getChildByName("gameplay") != nullptr){
        detachFromScene();
    }
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
    _scene->addChildWithName(_gameplayNode, "gameplay");
}

void GameState::detachFromScene(){
    _scene->removeChild(_gameplayNode);
}

std::shared_ptr<GameObject> GameState::getActiveCharacter()
{
    if (_playerCharacters.size() < NUM_PLAYER_CHARS * 2){
        return nullptr;
    }
    return _playerCharacters.at(_activeCharacterPosition);
}

std::vector<std::shared_ptr<GameObject>> GameState::getInactiveCharacters(){
    std::vector<std::shared_ptr<GameObject>> inactives = std::vector<std::shared_ptr<GameObject>>();
    for (int i = 0; i<getPlayerCharacters().size();++i){
        if (i == _activeCharacterPosition){
            continue;
        }
        
        std::shared_ptr<GameObject> playerOb = getPlayerCharacters().at(i);
        if (playerOb->type != GameObject::ObjectType::CHARACTER){
            continue;
        }
        
        inactives.push_back(playerOb);
    }
    return inactives;
}

std::shared_ptr<GameObject> GameState::getInactiveCharacter(){
    for (int i = 0; i<getPlayerCharacters().size();++i){
        if (i == _activeCharacterPosition){
            continue;
        }
        
        std::shared_ptr<GameObject> playerOb = getPlayerCharacters().at(i);
        if (playerOb->type != GameObject::ObjectType::CHARACTER){
            continue;
        }
        
        return playerOb;
    }
    return nullptr;
}

void GameState::toggleActiveCharacter()
{
    for (int i =1;i<=_playerCharacters.size();i++){
        int charIndex = (_activeCharacterPosition + i) % _playerCharacters.size();
        if (_playerCharacters.at(charIndex)->type == GameObject::ObjectType::CHARACTER){
            _activeCharacterPosition = charIndex;
            break;
        }
    }
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

size_t GameState::getNumberEnemyCharacters(){
    return _enemyObjects.size();
}

int getClosestCharIndex(std::vector<std::shared_ptr<GameObject>> objs, Vec2 physCoord){
    if (objs.size() <= 0){
        return -1;
    }
    float minDist = FLT_MAX;
    int closerIndex = -1;
    for (int i=0 ; i<objs.size() ; ++i){
        std::shared_ptr<GameObject> playerOb = objs.at(i);
        if (playerOb->type != GameObject::ObjectType::CHARACTER){
            continue;
        }
        
        float playerDist = playerOb->getPosition().distance(physCoord);
        if (playerDist < minDist){
            closerIndex = i;
            minDist = playerDist;
        }
    }
    return closerIndex;
}


std::shared_ptr<GameObject> GameState::getClosestChar(Vec2 phys_coord) {
    
    int closerIndex = getClosestCharIndex(_playerCharacters, phys_coord);
    return _playerCharacters.at(closerIndex);
};

void GameState::setClosestChar(cugl::Vec2 phys_coord) {
    int closerIndex = getClosestCharIndex(_playerCharacters, phys_coord);
    _activeCharacterPosition = closerIndex;
};


#pragma mark Coordinate Conversions

float GameState::getPhysicsScale(){
    return GAME_PHYSICS_SCALE;
}

std::shared_ptr<GameObject> GameState::getUID2GameObject(int uid) {
	// search
	for (auto entry : getPlayerCharacters()) {
		if (entry->getUid() == uid) { return entry; }
	}
	for (auto entry : getEnemyObjects()) {
		if (entry->getUid() == uid) { return entry; }
	}
    return nullptr;
}

