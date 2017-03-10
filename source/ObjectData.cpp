//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"

using namespace cugl;

bool ObjectData::init(int uid,int shape_id, int speed,int acceleration,Element element){
    this->_uid = uid;
    this->shape_id = shape_id;
    this->speed = speed;
    this->acceleration = acceleration;
    this->element = element;
    return true;
};


std::string ObjectData::serialize(){
    return "";
}

bool ObjectData::preload(const std::string& file){
	std::cout << file.c_str() << std::endl;
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool ObjectData::materialize(){
    return true;
}
