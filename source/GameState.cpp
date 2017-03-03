//
//  GameState.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameState.hpp"

using namespace cugl;

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

/** The name of the space texture */
// HACK replace with level loading sending event
#define BACKGROUND_TEXTURE       "bg_dark_lake"
#define PLAYER_BOY_YELLOW          "player_boy"
#define PLAYER_GIRL_BLUE         "player_girl"

bool GameState::init(const std::shared_ptr<cugl::AssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    
    reset = false;
    
    // Create the scene graph
    Size dimen = Application::get()->getDisplaySize();
//    dimen *= GAME_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
    _scene = Scene::alloc(dimen.width, dimen.height);
    
    // Get the space background.  Its size determines all scaling.
    auto image = assets->get<Texture>(BACKGROUND_TEXTURE);
    
    _bgnode = Node::alloc();
    _bgnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgnode->setPosition(Vec2::ZERO);
    
    auto bkgdTextureNode = PolygonNode::allocWithTexture(image);
    bkgdTextureNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgdTextureNode->setPosition(Vec2::ZERO);
    _bgnode->addChild(bkgdTextureNode);
    
    _worldnode = Node::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(Vec2::ZERO);
    
    _debugnode = Node::alloc();
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setPosition(Vec2::ZERO);
    
    _scene->addChild(_bgnode,0);
    _scene->addChild(_worldnode,1);
    _scene->addChild(_debugnode,2);
    
    // create the playable character gameObjects
    // the box2d Obstacle will be created in the collisionController init
    // and then attached to the gameObject
    
    Vec2 charGirlPos = Vec2::Vec2(300,300);
    image = assets->get<Texture>(PLAYER_GIRL_BLUE);
    auto charGirlNode = PolygonNode::allocWithTexture(image);
    charGirlNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    charGirlNode->setPosition(charGirlPos);
    
    std::shared_ptr<GameObject> charGirl = GameObject::alloc(charGirlNode);
    
    // HACK we should not set uid here we need to set uid from the data file
    // after we are loading the player character from data file as well the uid will
    // be unique
    charGirl->setUid(0);
    charGirl->setIsPlayer(true);
    
    auto boxGirl = BoxObstacle::alloc(charGirl->getNode()->getPosition(), charGirl->getNode()->getSize());
    std::shared_ptr<PhysicsComponent> physicsGirl = PhysicsComponent::alloc(boxGirl, Element::BLUE);
    charGirl->setPhysicsComponent(physicsGirl);
    
    Vec2 charBoyPos = Vec2::Vec2(150,150);
    image = assets->get<Texture>(PLAYER_BOY_YELLOW);
    auto charBoyNode = PolygonNode::allocWithTexture(image);
    charBoyNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    charBoyNode->setPosition(charBoyPos);
    std::shared_ptr<GameObject> charBoy = GameObject::alloc(charBoyNode);
    
    // HACK we should not set uid here we need to set uid from the data file
    // after we are loading the player character from data file as well the uid will
    // be unique
    charBoy->setUid(1);
    charBoy->setIsPlayer(true);
    
    auto boxBoy = BoxObstacle::alloc(charBoy->getNode()->getPosition(), charBoy->getNode()->getSize());
    std::shared_ptr<PhysicsComponent> physicsBoy = PhysicsComponent::alloc(boxBoy, Element::GOLD);
    charBoy->setPhysicsComponent(physicsBoy);
    
    _playerCharacters.push_back(charBoy);
    _playerCharacters.push_back(charGirl);
    
    _activeCharacterPosition = 0;
    
    return true;
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

