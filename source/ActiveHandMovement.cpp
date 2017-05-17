//
//  ActiveHandMovement.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveHandMovement.hpp"
#include "GameState.hpp"
#include "MoveController.hpp"

using namespace cugl;

bool ActiveHandMovement::init(std::shared_ptr<GenericAssetManager> assets, std::shared_ptr<HandMovementComponent> c){
    _component = c;
    std::shared_ptr<Texture> t = assets->get<Texture>(_component->_textureKey);
    _activePath = ActivePath::alloc(_component->_path);
    _node = PolygonNode::allocWithTexture(t);
    if (_component->_path->_coordinates.size() <= 0){
        return false;
    }
    _node->setPosition(_component->_path->get(0));
    
    return true;
}

bool ActiveHandMovement::update(){
    Vec2 current = _node->getPosition();
    Vec2 goal = _activePath->_path->get(_activePath->_pathIndex);
    Vec2 velocity = MoveController::getVelocityVector(current, goal, _component->_speed);
    
    Vec2 newPos = current + velocity;
    _node->setPosition(newPos);
    if (_activePath->updateActivePath(newPos)){
        if (_component->_repeat){
            reset();
            return false;
        }
        return true;
    }
    return false;
}

void ActiveHandMovement::reset(){
    _activePath->_pathIndex = 0;
    _node->setPosition(_component->_path->get(0));
}
