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
        vertices[i] = this->vertices[i];
    }
    return true;
}

std::shared_ptr<JsonValue> ShapeData::toJsonValue()
{
	std::shared_ptr<JsonValue> verticesArray = JsonValue::allocArray();
	for (int i = 0; i < this->vertices.size(); i++)
	{
		verticesArray->appendValue(this->vertices[i]);
	}
	std::shared_ptr<JsonValue> vertices = Data::toJsonValue();
	vertices->appendChild("vertices", verticesArray);
	return vertices;
}

bool ShapeData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ShapeData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    float scale = 1.0;
    if(json->has("scale")){
        scale = json->getFloat("scale");
    }
    std::vector<float> arr = json->get("vertices")->asFloatArray();
    std::vector<float> scaled;
    for(int i = 0; i < arr.size(); i++){
        scaled.push_back(arr.at(i) * scale);
    }
	init(scaled);
	Data::preload(json);

	return true;
}

bool ShapeData::materialize(){
    return true;
}
