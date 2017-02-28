//
//  AnimationData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationData.hpp"

using namespace cugl;

std::string AnimationData::serialize(){
    return "";
}

bool AnimationData::preload(const std::string& file){
    return true;
}

bool AnimationData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool AnimationData::materialize(){
    return true;
}
