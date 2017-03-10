//
//  pathData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "pathData.hpp"

using namespace cugl;

std::string PathData::serialize(){
    return "";
}

bool PathData::preload(const std::string& file){
	std::cout << file.c_str() << std::endl;
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool PathData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    return true;
}

bool PathData::materialize(){
    return true;
}
