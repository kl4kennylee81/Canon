//
//  HandMovementComponent.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "HandMovementComponent.hpp"
#include "Util.hpp"

using namespace cugl;

bool HandMovementComponent::init(std::shared_ptr<cugl::JsonValue> json){
    _uiDataKey = json->getString("uiKey");
    _path = Path::alloc(Path::getPathFromString(json->getString("path")));
    _repeat = json->getBool("repeat");
    _speed = json->getFloat("speed");
    _cooldown = json->getFloat("cooldown");
    return true;
}
