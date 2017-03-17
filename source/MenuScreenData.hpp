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
#include "Data.hpp"

using std::string;

class MenuEntry {
public:
	string name;
	boolean unlocked;
	boolean complete;
	int highScore;

	MenuEntry() {}

	bool init(string n, boolean unloc, boolean com, int high) {
		this->name = n;
		this->unlocked = unloc;
		this->complete = com;
		this->highScore = high;
		return true;
	}

	static std::shared_ptr<MenuEntry> alloc(string n, boolean unloc, boolean com, int high) {
		std::shared_ptr<MenuEntry> result = std::make_shared<MenuEntry>();
		return (result->init(n, unloc, com, high) ? result : nullptr);
	}
};

class MenuScreenData : Data {
public:
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* MenuScreenData_hpp */
