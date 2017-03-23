//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"

using namespace cugl;

bool ObjectData::init(std::string shapeKey,std::string animKey,std::string aiKey, Element element){
    this->shapeKey = shapeKey;
    this->animationKey = animKey;
    this->aiKey = aiKey;
    this->element = element;
    return true;
};


std::string ObjectData::serialize(){
    return "";
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
    std::string aiKey = json->getString("aiKey");
	auto el = (json->getString("element") == "BLUE") ? Element::BLUE : Element::GOLD;
    init(sKey,animKey,aiKey,el);
    return true;
}

bool ObjectData::materialize(){
    return true;
}
