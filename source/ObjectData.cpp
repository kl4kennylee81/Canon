//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"
using namespace cugl;

bool ObjectData::init(std::string shapeKey,std::string animKey){
    this->shapeKey = shapeKey;
    this->animationKey = animKey;
    return true;
};

std::shared_ptr<JsonValue> ObjectData::toJsonValue()
{
	std::shared_ptr<JsonValue> object = JsonValue::allocObject();
	object->appendChild("shapeKey", JsonValue::alloc(shapeKey));
	object->appendChild("animationKey", JsonValue::alloc(animationKey));
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
    std::string animKey = json->getString("animationKey");
    init(sKey,animKey);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
