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
	std::cout << file.c_str() << std::endl;
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool WaveData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	for (int i = 0; i < json->size(); i++) {
		auto child = json->get(i);
		auto entry = WaveEntry::alloc(child->getInt("objectKey"), child->getFloat("x"), child->getFloat("y"));
		addWaveEntry(entry);
	}
	init(0);
    return true;
}

bool WaveData::materialize(){
    return true;
}
