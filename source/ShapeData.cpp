//
//  ShapeData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ShapeData.hpp"
// TODO included for the GAME_PHYSICS_SCALE may consider not doing it like this
#include "GameState.hpp"

using namespace cugl;

std::string ShapeData::serialize(){
    return "";
}

bool ShapeData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ShapeData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	init(json->getInt("id"), json->get("vertices")->asFloatArray());
    
    // convert the vertices from world to physics coordinates since our game uses physics Coordinates logically
    for (int i = 0;i<vertices.size() ;i++){
        vertices[i] = vertices[i]/GAME_PHYSICS_SCALE;
    }
	return true;
}

bool ShapeData::materialize(){
    return true;
}
