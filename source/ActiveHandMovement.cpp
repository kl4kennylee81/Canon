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
#include "UIData.hpp"

using namespace cugl;

bool ActiveHandMovement::init(std::shared_ptr<GenericAssetManager> assets,
                              std::shared_ptr<HandMovementComponent> c,
                              std::map<std::string,std::string> fontMap){
    _cooldownFrames = 0;
    _component = c;
    _activePath = ActivePath::alloc(_component->_path);
    
    std::shared_ptr<UIData> uiData = assets->get<UIData>(c->_uiDataKey);
    _node = uiData->dataToNode(assets,fontMap);
    if (_component->_path->_coordinates.size() <= 0){
        return false;
    }
    _node->setPosition(_component->_path->get(0));
    
    return true;
}

bool ActiveHandMovement::update(){
    if (_activePath->isDone()){
        if (_component->_repeat){
            _cooldownFrames+=1;
        }
        
        if (_cooldownFrames >= _component->_cooldown) {
            _cooldownFrames = 0;
            reset();
        }
        return !_component->_repeat;
    }
    
    Vec2 current = _node->getPosition();
    Vec2 goal = _activePath->_path->get(_activePath->_pathIndex);
    Vec2 velocity = MoveController::getVelocityVector(current, goal, _component->_speed);
    
    Vec2 newPos = current + velocity;
    _node->setPosition(newPos);
    if (_activePath->updateActivePath(newPos)){
        if (_component->_repeat){
            _node->setVisible(false);
        }
        return !_component->_repeat;
    }
    return false;
}

void ActiveHandMovement::reset(){
    _activePath->_pathIndex = 0;
    _node->setPosition(_component->_path->get(0));
    _node->setVisible(true);
}
