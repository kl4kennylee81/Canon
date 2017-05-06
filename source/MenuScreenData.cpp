//
//  MenuScreenData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuScreenData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> MenuScreenData::toJsonValue()
{
	return JsonValue::allocNull();
}

bool MenuScreenData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool MenuScreenData::preload(const std::shared_ptr<cugl::JsonValue>& json){
	std::string mKey = json->getString("menuKey");
	std::string mbKey = json->getString("menuBackgroundKey");
	std::vector<std::string> entries;

	for (int i = 0; i < json->get("UIEntries")->size(); i++) {
		auto entry = json->get("UIEntries")->get(i)->key();
		entries.push_back(entry);
	}

	init(mKey, mbKey, entries);
	return true;
}

void MenuScreenData::addUIEntry(std::string s) {
	_uiEntryKeys.push_back(s);
}

