//
//  BulletAI.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletAI.hpp"
#include "MoveController.hpp"

using namespace cugl;

void BulletAI::update(std::shared_ptr<GameState> state) {
    Vec2 vec = Vec2::forAngle(angle);
    vec.negate();
    vec.scale(velocity);
    vec.scale(GameState::_internalClock->getTimeDilation());
    _object->getPhysicsComponent()->getBody()->setLinearVelocity(vec);
}

bool BulletAI::garbageCollect(GameObject* obj) {
    return true;
}

bool BulletAI::isActive(){
    return _isActive;
}

void BulletAI::toggleActive(){
    _isActive = !_isActive;
}
