//
//  BulletAI.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletAI.hpp"

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

std::shared_ptr<cugl::JsonValue> BulletAI::toJsonValue(){
    std::shared_ptr<JsonValue> json = JsonValue::allocObject();
    
    // duplicate code from move controller rip
    json->appendChild("isActive", JsonValue::alloc(_isActive));
    json->appendChild("velocity", JsonValue::alloc(velocity));
    json->appendChild("angle", JsonValue::alloc(angle));
    json->appendChild("uid", JsonValue::alloc(std::to_string(_object->getUid())));
    json->appendChild("aiType", cugl::JsonValue::alloc("BULLET"));
    return json;
}

bool BulletAI::initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state) {
    int uid = std::stoi(json->getString("uid"));
    std::shared_ptr<GameObject> gObj = state->getUID2GameObject(uid);
    
    float vel = json->getFloat("velocity");
    float ang = json->getFloat("angle");
    init(gObj, vel,ang);
    return true;
    
}
