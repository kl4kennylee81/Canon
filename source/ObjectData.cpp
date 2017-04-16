//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"
using namespace cugl;

std::string ObjectData::getShapeKey(Element e){
    if (e == Element::BLUE){
        return this->blueShapeKey;
    } else {
        return this->goldShapeKey;
    }
}

std::string ObjectData::getAnimationKey(Element e){
    if (e == Element::BLUE){
        return this->blueAnimationKey;
    } else {
        return this->goldAnimationKey;
    }
}

bool ObjectData::init(std::string blueShapeKey,std::string blueAnimKey,std::string goldShapeKey, std::string goldAnimKey){
    this->blueShapeKey = blueShapeKey;
    this->blueAnimationKey = blueAnimKey;
    this->goldShapeKey = goldShapeKey;
    this->goldAnimationKey = goldAnimKey;
    return true;
};

std::shared_ptr<JsonValue> ObjectData::toJsonValue()
{
	std::shared_ptr<JsonValue> object = JsonValue::allocObject();
	object->appendChild("blueShapeKey", JsonValue::alloc(blueShapeKey));
	object->appendChild("blueAnimationKey", JsonValue::alloc(blueAnimationKey));
    object->appendChild("goldShapeKey", JsonValue::alloc(goldShapeKey));
    object->appendChild("goldAnimationKey", JsonValue::alloc(goldAnimationKey));
	return object;
}

bool ObjectData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    std::string bsKey = json->getString("blueShapeKey");
    std::string banimKey = json->getString("blueAnimationKey");
    std::string gsKey = json->getString("goldShapeKey");
    std::string ganimKey = json->getString("goldAnimationKey");
    init(bsKey,banimKey,gsKey,ganimKey);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
