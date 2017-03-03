//
//  ObjectData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ObjectData.hpp"

using namespace cugl;

bool ObjectData::init(int path_id,int shape_id, int speed,int acceleration,Element element){
    path_id = path_id;
    shape_id = shape_id;
    speed = speed;
    acceleration = acceleration;
    element = element;
    return true;
};


std::string ObjectData::serialize(){
    return "";
}

bool ObjectData::preload(const std::string& file){
    return true;
}

bool ObjectData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool ObjectData::materialize(){
    return true;
}
