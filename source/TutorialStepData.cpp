//
//  TutorialStepData.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialStepData.hpp"

using namespace cugl;


bool TutorialStepData::preload(const std::string& file){
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    preload(json);
    return true;
}

bool TutorialStepData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    Data::preload(json);
    std::string mbKey = json->getString("menuBackgroundKey");
    std::vector<std::string> entries = json->get("UIEntries")->asStringArray();
    TutorialTransition start = strToTransition(json->getString("start"));
    TutorialTransition end = strToTransition(json->getString("end"));
    bool activeWhenReset = json->getBool("activeWhenReset",false);
    
    float minTime = json->getFloat("minTime");
    
    std::shared_ptr<HandMovementComponent> hmovement = nullptr;
    if (json->get("handMovement") != nullptr){
        hmovement = HandMovementComponent::alloc(json->get("handMovement"));
    }
    
    std::vector<TutorialEffect> startEffects;
    if (json->get("startEffects") != nullptr){
        std::vector<std::string> effectStrs = json->get("startEffects")->asStringArray();
        
        // convert the strings to tutorial effects
        for (std::string effectStr : effectStrs){
            startEffects.push_back(strToTutorialEffect(effectStr));
        }
    }
    
    std::vector<TutorialEffect> endEffects;
    if (json->get("startEffects") != nullptr){
        std::vector<std::string> effectStrs = json->get("endEffects")->asStringArray();
        
        // convert the strings to tutorial effects
        for (std::string effectStr : effectStrs){
            endEffects.push_back(strToTutorialEffect(effectStr));
        }
    }
    
    std::vector<std::string> hints;
    if (json->get("hints") != nullptr){
        hints = json->get("hints")->asStringArray();
    }
    
    return init(mbKey,entries,start,end,startEffects,endEffects,minTime,hmovement,hints,activeWhenReset);
}

std::shared_ptr<cugl::JsonValue> TutorialStepData::toJsonValue(){
    return JsonValue::allocNull();
}
