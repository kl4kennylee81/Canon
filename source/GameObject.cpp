//
//  GameObject.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameObject.hpp"

using namespace cugl;

bool GameObject::init(){
    _isPlayer = false;
    return true;
}

bool GameObject::init(std::shared_ptr<cugl::Node> node){
    _node = node;
    _isPlayer = false;
    return true;
}

bool GameObject::init(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
    setPhysicsComponent(body);
    _node = node;
    _isPlayer = false;
    return true;
}

void GameObject::setNode(std::shared_ptr<cugl::Node> node){
    _node = node;
}

void GameObject::setPhysicsComponent(std::shared_ptr<PhysicsComponent> body){
    _body = body;
    // a sensor will return to the collision detection but won't apply collision the internal
    // box2d kinematics physics on it.
    _body->getBody()->setDensity(0.0f);
    _body->getBody()->setFriction(0.0f);
    _body->getBody()->setRestitution(0.0f);
    _body->getBody()->setSensor(true);
}

void GameObject::sync(){
    _node->setPosition(_body->getBody()->getPosition());
}

