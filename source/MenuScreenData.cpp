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

bool MenuEntry::init(const std::shared_ptr<cugl::JsonValue>& json)
{
	menuKey = json->getString("menuKey");
	menuBackgroundKey = json->getString("backgroundKey");
	_uiEntryKeys = (json->get("UIEntries")->asStringArray());
	return true;
}

std::shared_ptr<cugl::JsonValue> MenuEntry::toJsonValue()
{
	return std::shared_ptr<cugl::JsonValue>();
}

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
	init();
	for (int i = 0; i < json->size(); i++) {
		auto child = json->get(i);
		auto entry = MenuEntry::alloc(child);
		addMenuEntry(entry);
	}
    _startMenuKey = json->getString("startMenuKey");
	return true;
}

bool MenuScreenData::materialize(){
    return true;
}

void MenuScreenData::addMenuEntry(std::shared_ptr<MenuEntry> w) {
    _menuEntries[w->menuKey] = w;
}

std::map<string, std::shared_ptr<MenuEntry>> MenuScreenData::getMenuEntries() {
    return _menuEntries;
}

std::shared_ptr<MenuEntry> MenuScreenData::getEntry(string key) {
    return _menuEntries[key];
}

