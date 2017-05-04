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

std::string ObjectData::getAnimationKey(ElementType e){
    if (e == ElementType::BLUE){
        return this->blueAnimationKey;
    } else {
        return this->goldAnimationKey;
    }
}

bool ObjectData::init(std::string shapeKey,std::string blueAnimKey, std::string goldAnimKey, std::string soundKey,float animScale){
    this->shapeKey = shapeKey;
    this->soundKey = soundKey;
    this->blueAnimationKey = blueAnimKey;
    this->goldAnimationKey = goldAnimKey;
    this->animationScale = animScale;
    return true;
};

float ObjectData::getAnimationScale(){
    return animationScale;
}

std::shared_ptr<JsonValue> ObjectData::toJsonValue()
{
	std::shared_ptr<JsonValue> object = JsonValue::allocObject();
	object->appendChild("shapeKey", JsonValue::alloc(shapeKey));
    object->appendChild("soundKey", JsonValue::alloc(soundKey));
	object->appendChild("blueAnimationKey", JsonValue::alloc(blueAnimationKey));
    object->appendChild("goldAnimationKey", JsonValue::alloc(goldAnimationKey));
    object->appendChild("animationScale", JsonValue::alloc(animationScale));
	return object;
}

bool ObjectData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string sKey = json->getString("shapeKey");
    std::string soundKey = json->getString("soundKey");
    std::string banimKey = json->getString("blueAnimationKey");
    std::string ganimKey = json->getString("goldAnimationKey");
    float animScale = json->getFloat("animationScale",1.0f);
    init(sKey,banimKey,ganimKey,soundKey,animScale);
	Data::preload(json);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
