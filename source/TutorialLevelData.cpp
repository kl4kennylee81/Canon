//
//  TutorialLevelData.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialLevelData.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> TutorialLevelData::toJsonValue()
{
    std::shared_ptr<JsonValue> tutorialJson = Data::toJsonValue();
    std::shared_ptr<JsonValue> stepList = JsonValue::allocObject();
    for(std::string stepDataKey : this->_stepKeys){
        stepList->appendChild(JsonValue::alloc(stepDataKey));
    }
    tutorialJson->appendChild("stepKeys",stepList);
    tutorialJson->appendChild("levelKey",JsonValue::alloc(this->_levelKey));
    
    std::shared_ptr<JsonValue> fontMapJson = JsonValue::allocObject();
    for(std::pair<std::string,std::string> const &ent : _fontMap) {
        std::string const &menuFont = ent.first;
        std::string const &actualFont = ent.second;
        fontMapJson->appendChild(menuFont,JsonValue::alloc(actualFont));
    }
    return tutorialJson;
}


bool TutorialLevelData::preload(const std::string& file)
{
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool TutorialLevelData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
    Data::preload(json);
	std::vector<std::string> sKeys = json->get("stepKeys")->asStringArray();
    std::string lKey = json->getString("levelKey");
    
    std::shared_ptr<JsonValue> fontMapJson = json->get("fontMap");
    
    std::map<std::string,std::string> tempFontMap;
    if (fontMapJson != nullptr){
        for (int i =0;i < fontMapJson->size();i++){
            std::shared_ptr<JsonValue> jsonEntry = fontMapJson->get(i);
            std::string uiDataFontKey = jsonEntry->key();
            std::string actualFontKey = fontMapJson->getString(jsonEntry->key());
            tempFontMap.insert(std::make_pair(uiDataFontKey,actualFontKey));
        }
    }
    
    return init(sKeys,lKey,tempFontMap);
}
