//
//  PhysicsComponent.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PhysicsComponent.hpp"

using namespace cugl;

PhysicsComponent::PhysicsComponent() :
_body(nullptr),
_speed(0),
_acceleration(0)
{}

cugl::Obstacle* PhysicsComponent::getBody(){
    return _body.get();
}

bool PhysicsComponent::init(std::shared_ptr<cugl::Obstacle> body){
    _body = body;
    _speed = 0;
    _acceleration = 0;
    return true;
}
