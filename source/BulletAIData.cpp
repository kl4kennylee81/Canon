//
//  BulletAIData.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletAIData.hpp"
#include "BulletAI.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> BulletAIData::toJsonValue() {
    std::shared_ptr<JsonValue> ai = JsonValue::allocObject();
    return ai;
}

bool BulletAIData::preload(const std::string& file) {
    return true;
}

bool BulletAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    return true;
}

bool BulletAIData::materialize() {
    return true;
}

std::shared_ptr<ActiveAI> BulletAIData::newActiveAI(std::shared_ptr<GameObject> object) {
    return BulletAI::alloc(object, velocity, angle);
}
