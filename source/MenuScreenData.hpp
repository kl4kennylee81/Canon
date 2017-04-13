//
//  MenuScreenData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MenuScreenData_hpp
#define MenuScreenData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "Data.hpp"
#include "UIData.hpp"

using std::string;

class MenuEntry {
public:
	string menuKey;
	string menuBackgroundKey;
	std::vector<string> _uiEntryKeys;

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

	bool init(const std::shared_ptr<cugl::JsonValue>& json);

	static std::shared_ptr<MenuEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
		std::shared_ptr<MenuEntry> result = std::make_shared<MenuEntry>();
		return (result->init(json) ? result : nullptr);
	}
};



class MenuScreenData : Data {
public:
	std::map<string, std::shared_ptr<MenuEntry>> _menuEntries;
	string startMenuKey = "startMenu"; // use this as key inside _menuEntries to get the initial menu

	MenuScreenData() : Data() {}

	bool init() {
		return true;
	}

	static std::shared_ptr<MenuScreenData> alloc() {
		std::shared_ptr<MenuScreenData> result = std::make_shared<MenuScreenData>();
		return (result->init() ? result : nullptr);
	}

	void addMenuEntry(std::shared_ptr<MenuEntry> w) {
		_menuEntries[w->menuKey] = w;
	}

	std::map<string, std::shared_ptr<MenuEntry>> getMenuEntries() {
		return _menuEntries;
	}

	std::shared_ptr<MenuEntry> getEntry(string key) {
		return _menuEntries[key];
	}

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* MenuScreenData_hpp */
