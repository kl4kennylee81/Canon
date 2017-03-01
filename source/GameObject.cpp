//
//  GameObject.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameObject.hpp"

using namespace cugl;

bool GameObject::init(std::unique_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
    _body = std::move(body);
    
    // a sensor will return to the collision detection but won't apply collision the internal
    // box2d kinematics physics on it.
    b2Fixture* fixture_list = _body->getBody()->getBody()->GetFixtureList();
    for( b2Fixture* fixture = fixture_list; fixture != NULL; fixture = fixture->GetNext() ) {
        fixture->SetSensor(true);
    }
    _node = node;
    return true;
}
