//
//  GameState.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameState.hpp"

float GameState::_physicsScale;

using namespace cugl;

/** The name of the space texture */
// HACK replace with level loading sending event
#define BACKGROUND_TEXTURE       "bg_dark_lake"
#define NUM_PLAYER_CHARS         2

bool GameState::init(const std::shared_ptr<GenericAssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    
    reset = false;
    
    // Create the scene graph
    Size size = Application::get()->getDisplaySize();
    
    size *= GAME_WIDTH/size.width; // Lock the game to a reasonable resolution
    
    // magic numbers are okay as long as 16:9
    _bounds = Rect::Rect(0,0,32,18);
    
    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    GameState::_physicsScale = size.width / _bounds.size.width;
    
//    std::cout << "physicsScale:" << _physicsScale << "\n";
    
    // Get the space background.  Its size determines all scaling.
    auto image = assets->get<Texture>(BACKGROUND_TEXTURE);
    auto bkgdTextureNode = PolygonNode::allocWithTexture(image);
    bkgdTextureNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgdTextureNode->setPosition(Vec2::ZERO);
    
    _bgnode = Node::alloc();
    _bgnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgnode->setPosition(Vec2::ZERO);
    _bgnode->addChild(bkgdTextureNode);
    
    _worldnode = Node::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(Vec2::ZERO);
    
    _debugnode = Node::alloc();
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setScale(GameState::_physicsScale);
    _debugnode->setPosition(Vec2::ZERO);
    
    _scene = Scene::alloc(size);
    _scene->addChild(_bgnode,0);
    _scene->addChild(_worldnode,1);
    _scene->addChild(_debugnode,2);
    
    // create the playable character gameObjects
    // the box2d Obstacle will be created in the collisionController init
    // and then attached to the gameObject
    
    #pragma mark : Player Girl
    std::shared_ptr<GameObject> charGirl = GameObject::alloc();
    
    // HACK we should not set uid here we need to set uid from the data file
    // after we are loading the player character from data file as well the uid will
    // be unique
    charGirl->setUid(0);
    charGirl->setIsPlayer(true);
    
    auto charGirlPos = Vec2::Vec2(16,9);
    auto charGirlSize = Size::Size(50,50);
    auto boxGirl = BoxObstacle::alloc(charGirlPos, charGirlSize / GameState::_physicsScale);
    std::shared_ptr<PhysicsComponent> physicsGirl = PhysicsComponent::alloc(boxGirl, Element::BLUE);
    charGirl->setPhysicsComponent(physicsGirl);
    
    #pragma mark : Player Boy
    
    std::shared_ptr<GameObject> charBoy = GameObject::alloc();
    
    // HACK we should not set uid here we need to set uid from the data file
    // after we are loading the player character from data file as well the uid will
    // be unique
    charBoy->setUid(1);
    charBoy->setIsPlayer(true);

    auto charBoyPos = Vec2::Vec2(15,12);
    auto charBoySize = Size::Size(50,50);
    auto boxBoy = BoxObstacle::alloc(charBoyPos, charBoySize / GameState::_physicsScale);
    std::shared_ptr<PhysicsComponent> physicsBoy = PhysicsComponent::alloc(boxBoy, Element::GOLD);
    charBoy->setPhysicsComponent(physicsBoy);
    
    _playerCharacters.push_back(charBoy);
    _playerCharacters.push_back(charGirl);
    
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

