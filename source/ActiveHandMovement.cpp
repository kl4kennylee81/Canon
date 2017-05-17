//
//  ActiveHandMovement.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveHandMovement.hpp"

using namespace cugl;

bool ActiveHandMovement::init(std::shared_ptr<GenericAssetManager> assets, std::shared_ptr<HandMovementComponent> component){
    component = component;
    std::shared_ptr<Texture> t = assets->get<Texture>(component->_textureKey);
    _node = PolygonNode::allocWithTexture(t);
    _activePath = ActivePath::alloc(component->_path);
    return true;
}
