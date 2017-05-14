//
//  TutorialStepData.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialStepData_hpp
#define TutorialStepData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include <map>
#include "Data.hpp"
#include "UIData.hpp"

class TutorialStepData : public Data {
private:
    std::vector<std::string> _uiEntryKeys;
    
public:
    std::string menuBackgroundKey; // replace the background for narrative control
    
    std::vector<std::string> getUIEntryKeys() {
        return _uiEntryKeys;
    }
    
    bool init(std::string mbKey, std::vector<std::string> keys) {
        menuBackgroundKey = mbKey;
        _uiEntryKeys = keys;
        return true;
    }
    
    static std::shared_ptr<TutorialStepData> alloc(std::string mbKey, std::vector<std::string> keys) {
        std::shared_ptr<TutorialStepData> result = std::make_shared<TutorialStepData>();
        return (result->init(mbKey, keys) ? result : nullptr);
    }
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<TutorialStepData> alloc() {
        std::shared_ptr<TutorialStepData> result = std::make_shared<TutorialStepData>();
        return (result->init() ? result : nullptr);
    }
    
    void addUIEntry(std::string s);
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
};

#endif /* TutorialStepData_hpp */
