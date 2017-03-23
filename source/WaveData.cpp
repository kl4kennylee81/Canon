//
//  WaveData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "WaveData.hpp"
#include "AIData.hpp"
#include "GameState.hpp" // for conversion to physicsScale

using namespace cugl;

std::string WaveData::serialize(){
    return "";
}

bool WaveData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool WaveData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init();
	for (int i = 0; i < json->size(); i++) {
		auto child = json->get(i);
		auto ai = AIData::alloc(child->getString("aiType"), child->getString("pathType"), child->getString("path"));
		auto entry = WaveEntry::alloc(
            child->getString("objectKey"),
			child->getFloat("x")/GAME_PHYSICS_SCALE,
            child->getFloat("y")/GAME_PHYSICS_SCALE);
		addWaveEntry(entry);
	}
    return true;
}

bool WaveData::materialize(){
    return true;
}
