//
//  BulletData.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletData.hpp"

using namespace cugl;

bool BulletData::init(std::string object, float start, float var, int num, float cool, float vel, bool tar){
    _objectKey = object;
    _startingAngle = start;
    _angleVariance = var;
    _numberShots = num;
    _cooldown = cool;
    _velocity = vel;
    _target = tar;
    return true;
}

std::shared_ptr<cugl::JsonValue> BulletData::toJsonValue(){
    return nullptr;
}

bool BulletData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool BulletData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string ob = json->getString("objectKey");
    float start = json->getFloat("startingAngle");
    float var = json->getFloat("angleVariance");
    int num = json->getInt("numberShots");
    float cool = json->getFloat("cooldown");
    float vel = json->getFloat("velocity");
    bool tar = json->getBool("target");
    init(ob, start, var, num, cool, vel, tar);
    return true;
}

bool BulletData::materialize(){
    return true;
}

std::shared_ptr<ActiveBullet> BulletData::newActiveBullet(std::shared_ptr<GameObject> object){
    return ActiveBullet::alloc(object);
}
