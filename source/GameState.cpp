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

bool GameState::init(const std::shared_ptr<cugl::AssetManager>& assets){
    if (assets == nullptr){
        return false;
    }
    
    // Create the scene graph
    Size dimen = Application::get()->getDisplaySize();
    dimen *= GAME_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
    _scene = Scene::alloc(dimen);
    
    // Get the space background.  Its size determines all scaling.
    auto bkgd = assets->get<Texture>(BACKGROUND_TEXTURE);
    
    _bgnode = Node::alloc();
    _bgnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgnode->setPosition(Vec2::ZERO);
    
    auto bkgdTextureNode = PolygonNode::allocWithTexture(bkgd);
    bkgdTextureNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgdTextureNode->setPosition(Vec2::ZERO);
    _bgnode->addChild(bkgdTextureNode);
    
    
    // Create the master node.  This, unlike the scene graph, is resolution independent.
    // If we do not do this, the children will not all line up correctly on different devices.
    Vec2 center(dimen.width/2.0f,dimen.height/2.0f);
    _worldnode = Node::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    _worldnode->setPosition(center);
    
    
    
    _scene->addChild(_bgnode,0);
    _scene->addChild(_worldnode,1);
    
    
    
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
