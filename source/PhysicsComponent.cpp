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
_acceleration(0),
_health(1)
{}

void PhysicsComponent::dispose(){
    _body = nullptr;
}

std::shared_ptr<cugl::PolygonObstacle> PhysicsComponent::getBody(){
    return _body;
}

bool PhysicsComponent::init(std::shared_ptr<cugl::PolygonObstacle> body, ElementType element, int health){
    _body = body;
    _speed = 0;
    _acceleration = 0;
    _elementType = element;
    _health = health;
    return true;
}

void PhysicsComponent::setBody(std::shared_ptr<cugl::PolygonObstacle> body){
    _body = body;
}

void PhysicsComponent::getHit(){
    _health--;
}

bool PhysicsComponent::isAlive(){
    return _health > 0;
}
