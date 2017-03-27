//
//  ShapeData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ShapeData.hpp" // GAME_PHYSICS_SCALE for conversion
#include "GameState.hpp"

using namespace cugl;

bool ShapeData::init(std::vector<float> v){
    this->vertices = v;
    
    for (int i = 0;i<this->vertices.size() ;i++){
        // converting from world coordinate sto physics
        vertices[i] = this->vertices[i]/GAME_PHYSICS_SCALE;
    }
    return true;
}

std::string ShapeData::serialize(){
	std::string serialized_string = "{\n\"vertices\": [";
	for (int i = 0; i < this->vertices.size(); i++)
	{
		serialized_string += std::to_string(this->vertices[i] * GAME_PHYSICS_SCALE) + "\n";
		if (i < this->vertices.size() - 1) serialized_string += ",";
	}
	serialized_string += "]}";
	return serialized_string;
}

bool ShapeData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ShapeData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	init(json->get("vertices")->asFloatArray());

	return true;
}

bool ShapeData::materialize(){
    return true;
}
