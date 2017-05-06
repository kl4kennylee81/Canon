//
//  MenuScreenData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuScreenData.hpp"
using namespace cugl;
using std::string;

std::shared_ptr<JsonValue> MenuScreenData::toJsonValue()
{
	return JsonValue::allocNull();
}

std::string MenuScreenData::serialize(){
    return toJsonValue()->toString();
}

bool MenuScreenData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool MenuScreenData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	// todo    
	return true;
}

bool MenuScreenData::materialize(){
    return true;
}

void MenuScreenData::addUIEntry(std::string s) {
	_uiEntryKeys.push_back(s);
}

