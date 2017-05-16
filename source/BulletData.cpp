//
//  BulletData.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletData.hpp"

using namespace cugl;

bool BulletData::init(std::string object, float start, float var, int num, int cool, float vel, bool tar, ElementDataType match){
    _objectKey = object;
    _startingAngle = start;
    _angleVariance = var;
    _numberShots = num;
    _cooldown = cool;
    _velocity = vel;
    _target = tar;
    _element = match;
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
    int cool = json->getInt("cooldown");
    float vel = json->getFloat("velocity");
    std::string tar = json->getString("target");
    bool tarb = tar.compare("true") == 0 ? true : false;
    std::string match = json->getString("element");
    ElementDataType el = Element::stringToElementDataType(match);
    
    init(ob, start, var, num, cool, vel, tarb, el);
    return true;
}

bool BulletData::materialize(){
    return true;
}

