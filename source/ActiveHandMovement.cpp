//
//  ActiveHandMovement.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveHandMovement.hpp"

using namespace cugl;

bool ActiveHandMovement::init(std::shared_ptr<HandMovementComponent> component){
    component = component;
    _node = PolygonNode::allocWithTexture(component->_texture);
    _activePath = ActivePath::alloc(component->_path);
    return true;
}
