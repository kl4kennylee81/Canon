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

class MenuScreenData : public Data {
private:
    std::map<std::string,std::string> _fontMap; // map menu font key to actual font assets
	std::vector<std::string> _uiEntryKeys;

public:
	std::string menuBackgroundKey;

	std::vector<std::string> getUIEntryKeys() {
		return _uiEntryKeys;
	}
    
    std::map<std::string,std::string> getFontMap(){
        return _fontMap;
    }

    bool init(std::string mbKey, std::vector<std::string> keys,std::map<std::string,std::string> fontMap) {
		menuBackgroundKey = mbKey;
		_uiEntryKeys = keys;
        _fontMap = fontMap;
		return true;
	}

	static std::shared_ptr<MenuScreenData> alloc(std::string mbKey, std::vector<std::string> keys,std::map<std::string,std::string> fontMap) {
		std::shared_ptr<MenuScreenData> result = std::make_shared<MenuScreenData>();
		return (result->init(mbKey, keys,fontMap) ? result : nullptr);
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
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
};

#endif /* MenuScreenData_hpp */
