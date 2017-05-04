//
//  Data.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Data.hpp"

using namespace cugl;

bool Data::preload(const std::string& file) {
    Asset::preload(file);
    return true;
}

bool Data::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    Asset::preload(json);
	key = json->getString("key");
    return true;
}

bool Data::materialize(){
    Asset::materialize();
    return true;
}
