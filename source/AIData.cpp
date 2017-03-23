//
//  WaveData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AIData.hpp"
#include "GameState.hpp" // needed for conversion to physicsScale
#include "Util.hpp"

using namespace cugl;


std::string AIData::serialize() {
	return "";
}

bool AIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

AIType AIData::getTypeFromString(const std::string& str) {
	if (str.compare("PATH") == 0) {
		return AIType::PATH;
	} 
	if (str.compare("HOMING") == 0) {
		return AIType::HOMING;
	}
	return AIType::STATIC;
}

PathType AIData::getPathTypeFromString(const std::string& str) {
	if (str.compare("HORIZONTAL") == 0) {
		return PathType::HORIZONTAL;
	}
	if (str.compare("VERTICAL") == 0) {
		return PathType::VERTICAL;
	}
	if (str.compare("CUSTOM") == 0) {
		return PathType::CUSTOM;
	}
	return PathType::NONE;
}

Vec2 AIData::getTupleFromString(const std::string& s) {
	std::string str = s.substr(1, s.size() - 2);
    auto nums = Util::split(str, ',');
	return Vec2::Vec2(std::stoi(nums.at(0)), std::stoi(nums.at(1)));
}

std::vector<Vec2> AIData::getPathFromString(const std::string& str) {
    auto strings = Util::split(str, ' ');
	std::vector<Vec2> path;
	for (auto it : strings) {
        // converting from world to physics coordinates
        path.push_back(getTupleFromString(it)/GAME_PHYSICS_SCALE);
	}
	return path;
}

bool AIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	AIType type = getTypeFromString(json->getString("type"));
	PathType pathType = getPathTypeFromString(json->getString("pathType"));
	std::vector<Vec2> path = getPathFromString(json->getString("path"));
	init(type, pathType, path);
	return true;
}

bool AIData::materialize() {
	return true;
}
