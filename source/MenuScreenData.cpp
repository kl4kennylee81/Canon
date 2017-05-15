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
    Data::preload(json);
    std::shared_ptr<JsonValue> fontMapJson = json->get("fontMap");
    
    std::map<std::string,std::string> tempFontMap;
    
    for (int i =0;i < fontMapJson->size();i++){
        std::shared_ptr<JsonValue> jsonEntry = fontMapJson->get(i);
        std::string uiDataFontKey = jsonEntry->key();
        std::string actualFontKey = jsonEntry->getString(jsonEntry->key());
        tempFontMap.insert(std::make_pair(uiDataFontKey,actualFontKey));
    }
    
	std::string mbKey = json->getString("menuBackgroundKey");
	std::vector<std::string> entries = json->get("UIEntries")->asStringArray();
	init(mbKey, entries,tempFontMap);
	return true;
}

void MenuScreenData::addUIEntry(std::string s) {
	_uiEntryKeys.push_back(s);
}

