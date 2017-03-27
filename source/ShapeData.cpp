//
//  ShapeData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ShapeData.hpp" // GAME_PHYSICS_SCALE for conversion
#include "GameState.hpp"
#include <iostream>
#include <fstream>

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
	std::shared_ptr<JsonValue> verticesArray = JsonValue::allocArray();
	for (int i = 0; i < this->vertices.size(); i++)
	{
		verticesArray->appendValue(this->vertices[i] * GAME_PHYSICS_SCALE);
	}
	std::shared_ptr<JsonValue> vertices = JsonValue::allocObject();
	vertices->appendChild("vertices", verticesArray);
	return vertices->toString();
}

bool ShapeData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool ShapeData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	init(json->get("vertices")->asFloatArray());
	std::string filename = "test_writer.json";
	Pathname path = Pathname(filename);
	std::shared_ptr<JsonWriter> writer = JsonWriter::alloc(path);
	writer->writeJson(json);

	std::ofstream myfile;
	myfile.open("test_writer2.json");
	myfile << this->serialize();
	myfile.close();

	return true;
}

bool ShapeData::materialize(){
    return true;
}
