//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"
using namespace cugl;

std::string ObjectData::getShapeKey(){
    return this->shapeKey;
}

std::string ObjectData::getSoundKey(){
    return this->soundKey;
}

float ObjectData::getSpeed(){
    return this->speed;
}

std::string ObjectData::getAnimationKey(ElementType e){
    if (e == ElementType::BLUE){
        return this->blueAnimationKey;
    } else {
        return this->goldAnimationKey;
    }
}

std::string ObjectData::getParticleStateKey(){
    return this->particleStateKey;
}

bool ObjectData::init(std::string shapeKey,std::string blueAnimKey, std::string goldAnimKey, std::string soundKey,float animScale,int health, float speed, std::string partStateKey){
    this->shapeKey = shapeKey;
    this->soundKey = soundKey;
    this->blueAnimationKey = blueAnimKey;
    this->goldAnimationKey = goldAnimKey;
    this->animationScale = animScale;
    this->health = health;
    this->speed = speed;
    this->particleStateKey = partStateKey;
    return true;
};

float ObjectData::getAnimationScale(){
    return animationScale;
}

int ObjectData::getHealth(){
    return health;
}

std::shared_ptr<JsonValue> ObjectData::toJsonValue()
{
	std::shared_ptr<JsonValue> object = Data::toJsonValue();
	object->appendChild("shapeKey", JsonValue::alloc(shapeKey));
    object->appendChild("soundKey", JsonValue::alloc(soundKey));
	object->appendChild("blueAnimationKey", JsonValue::alloc(blueAnimationKey));
    object->appendChild("goldAnimationKey", JsonValue::alloc(goldAnimationKey));
    object->appendChild("animationScale", JsonValue::alloc(animationScale));
    object->appendChild("health", JsonValue::alloc(static_cast<float>(health)));
    object->appendChild("particleStateKey", JsonValue::alloc(particleStateKey));
	return object;
}

bool ObjectData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	Data::preload(json);
    std::string sKey = json->getString("shapeKey");
    std::string soundKey = json->getString("soundKey");
    std::string banimKey = json->getString("blueAnimationKey");
    std::string ganimKey = json->getString("goldAnimationKey");
    std::string partKey = json->getString("particleStateKey");
    float speed = 0;
    if(json->has("speed")) {
        speed = json->getFloat("speed");
    }
    float animScale = json->getFloat("animationScale",1.0f);
    int health = json->getInt("health",1);
    init(sKey,banimKey,ganimKey,soundKey,animScale,health, speed,partKey);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
