//
//  GameObject.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameObject.hpp"

int GameObject::_atomicUidCounter(0);

using namespace cugl;

void GameObject::dispose(){
    _uid = 0;
    _body = nullptr;
    _node = nullptr;
    _isPlayer = false;
}

bool GameObject::init(){
    _uid = GameObject::getAtomicUid();
    _isPlayer = false;
    type = ObjectType::CHARACTER;
    return true;
}

bool GameObject::init(std::shared_ptr<cugl::Node> node){
    init();
    _node = node;
    return true;
}

bool GameObject::init(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
    init(node);
    setPhysicsComponent(body);
    return true;
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

void GameObject::sync(float scale){
    _node->setPosition(_body->getBody()->getPosition() * scale);
}

