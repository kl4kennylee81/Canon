//
//  GameState.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameState.hpp"

float GameState::_physicsScale;
InternalClock GameState::_internalClock;

using namespace cugl;

/** The name of the space texture */
// HACK replace with level loading sending event
#define BACKGROUND_TEXTURE       "bg_dark_lake"
#define NUM_PLAYER_CHARS         2

bool GameState::init(const std::shared_ptr<GenericAssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    
    GameState::_internalClock = InternalClock();
    
    // Create the scene graph
    Size size = Application::get()->getDisplaySize();
    
    size *= GAME_SCENE_WIDTH/size.width; // Lock the game to a reasonable resolution
    
    // magic numbers are okay as long as 16:9
    _bounds = Rect::Rect(0,0,32,18);
    
    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    GameState::_physicsScale = size.width / _bounds.size.width;
    
    float world_yPos = (size.height - (_bounds.getMaxY() * _physicsScale))/2;
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
    _worldnode->setScale(GameState::_physicsScale);
    _worldnode->setPosition(world_pos);
    
    _debugnode = Node::alloc();
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setScale(GameState::_physicsScale);
    _debugnode->setPosition(world_pos);
    
    _scene = Scene::alloc(size);
    _scene->addChild(_bgnode,0);
    _scene->addChild(_worldnode,1);
    _scene->addChild(_debugnode,2);
    
    // set the initial character position to 0
    _activeCharacterPosition = 0;
    
    return true;
}

std::shared_ptr<GameObject> GameState::getActiveCharacter()
{
    if (_playerCharacters.size() != NUM_PLAYER_CHARS){
        return nullptr;
    }
    return _playerCharacters.at(_activeCharacterPosition);
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

/** Helper function to calculate the y translate needed to go from scene to world **/
float GameState::getSceneToWorldTranslateY(){
    float sceneYMax = this->getScene()->getCamera()->getViewport().getMaxY();
    float scene_yPos = (sceneYMax - (_bounds.getMaxY() * GameState::getPhysicsScale()))/2.f;
    return scene_yPos;
}

/** Physics Conversion **/
Vec2& GameState::physicsToSceneCoords(Vec2& physicsCoords,Vec2& sceneCoords){
    Vec2::scale(physicsCoords, _physicsScale, &sceneCoords);
    sceneCoords.y += getSceneToWorldTranslateY();
    return sceneCoords;
}

Vec2& GameState::physicsToScreenCoords(Vec2& physicsCoords, Vec2& screenCoords){
    physicsToSceneCoords(physicsCoords,screenCoords);
    sceneToScreenCoords(screenCoords,screenCoords);
    return screenCoords;
}

/** Screen conversions */
Vec2& GameState::screenToSceneCoords(cugl::Vec2& screenCoords, cugl::Vec2& sceneCoords){
    Vec2 scene_pos = getScene()->getCamera()->screenToWorldCoords(screenCoords);
    sceneCoords.set(scene_pos);
    return screenCoords;
}

Vec2& GameState::screenToPhysicsCoords(cugl::Vec2& screenCoords, cugl::Vec2& physicsCoords){
    screenToSceneCoords(screenCoords,physicsCoords);
    sceneToPhysicsCoords(physicsCoords, physicsCoords);
    return physicsCoords;
}

/** Scene conversions */
Vec2& GameState::sceneToScreenCoords(cugl::Vec2& sceneCoords, cugl::Vec2& screenCoords){
    Vec2 screen_pos = getScene()->getCamera()->worldToScreenCoords(sceneCoords);
    screenCoords.set(screen_pos);
    return screenCoords;
}

Vec2& GameState::sceneToPhysicsCoords(cugl::Vec2& sceneCoords, cugl::Vec2& physicsCoords){
    physicsCoords.set(sceneCoords);
    physicsCoords.y -= getSceneToWorldTranslateY();
    Vec2::divide(physicsCoords,_physicsScale,&physicsCoords);
    return physicsCoords;
}




