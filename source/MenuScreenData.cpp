//
//  MenuScreenData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuScreenData.hpp"

using namespace cugl;

std::string MenuScreenData::serialize(){
    return "";
}

bool MenuScreenData::preload(const std::string& file){
    return true;
}

bool MenuScreenData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool MenuScreenData::materialize(){
    return true;
}
