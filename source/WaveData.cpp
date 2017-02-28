//
//  WaveData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "WaveData.hpp"

using namespace cugl;

std::string WaveData::serialize(){
    return "";
}

bool WaveData::preload(const std::string& file){
    return true;
}

bool WaveData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool WaveData::materialize(){
    return true;
}
