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
#define BACKGROUND_TEXTURE       "bg_floating_crystal"
#define PLAYER_CHAR_ONE          "model_player_char_one"
#define PLAYER_CHAR_TWO          "model_player_char_two"

bool GameState::init(const std::shared_ptr<cugl::AssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    
    // Create the scene graph
    Size dimen = Application::get()->getDisplaySize();
    dimen *= GAME_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
    _scene = Scene::alloc(dimen);
    
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
    
    _scene->addChild(_bgnode,0);
    _scene->addChild(_worldnode,1);
    
    // create the playable character gameObjects
    // the box2d Obstacle will be created in the collisionController init
    // and then attached to the gameObject
    
    Vec2 char1Pos = Vec2::Vec2(300,300);
    char1Pos.add(Vec2::Vec2(0,0));
    image = assets->get<Texture>(PLAYER_CHAR_ONE);
    auto player1Node = PolygonNode::allocWithTexture(image);
    player1Node->setAnchor(Vec2::ANCHOR_MIDDLE);
    player1Node->setPosition(char1Pos);
    std::shared_ptr<GameObject> player1 = GameObject::alloc(player1Node);
    
    
    Vec2 char2Pos = Vec2::Vec2(150,150);
    image = assets->get<Texture>(PLAYER_CHAR_TWO);
    auto player2Node = PolygonNode::allocWithTexture(image);
    player2Node->setAnchor(Vec2::ANCHOR_MIDDLE);
    player2Node->setPosition(char2Pos);
    std::shared_ptr<GameObject> player2 = GameObject::alloc(player2Node);
    
    _worldnode->addChild(player1Node,1);
    _worldnode->addChild(player2Node,1);
    
    _playerCharacters.push_back(player1);
    _playerCharacters.push_back(player2);
    
    _allObjects.push_back(player1);
    _allObjects.push_back(player2);
    
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
