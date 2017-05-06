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

class MenuScreenData : Data {

public:
	std::string menuKey;
	std::string menuBackgroundKey;
	std::vector<std::string> _uiEntryKeys;

	std::vector<std::string> getUIEntryKeys() {
		return _uiEntryKeys;
	}

	bool init(const std::shared_ptr<cugl::JsonValue>& json) {
		menuKey = json->getString("menuKey");
		menuBackgroundKey = json->getString("menuBackgroundKey");
		_uiEntryKeys = (json->get("UIEntries")->asStringArray());
		return true;
	}

	static std::shared_ptr<MenuScreenData> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
		std::shared_ptr<MenuScreenData> result = std::make_shared<MenuScreenData>();
		return (result->init(json) ? result : nullptr);
	}

	bool init() {
		return true;
	}

	static std::shared_ptr<MenuScreenData> alloc() {
		std::shared_ptr<MenuScreenData> result = std::make_shared<MenuScreenData>();
		return (result->init() ? result : nullptr);
	}

    void addUIEntry(std::string s);

	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* MenuScreenData_hpp */
