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
#include "TutorialEnums.hpp"

class TutorialStepData : public Data {
private:
    std::vector<std::string> _uiEntryKeys;
    
public:
    std::string menuBackgroundKey; // replace the background for narrative start screens
    
    TutorialTransition _startCondition;
    TutorialTransition _endCondition;
    
    std::vector<std::string> getUIEntryKeys() {
        return _uiEntryKeys;
    }
    
    bool init(std::string mbKey, std::vector<std::string> keys,TutorialTransition start,TutorialTransition end) {
        menuBackgroundKey = mbKey;
        _uiEntryKeys = keys;
        _startCondition = start;
        _endCondition = end;
        return true;
    }
    
    static std::shared_ptr<TutorialStepData> alloc(std::string mbKey, std::vector<std::string> keys,
                                                   TutorialTransition start,TutorialTransition end) {
        std::shared_ptr<TutorialStepData> result = std::make_shared<TutorialStepData>();
        return (result->init(mbKey, keys,start,end) ? result : nullptr);
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
    
    TutorialTransition getStartCondition(){
        return _startCondition;
    }
    
    TutorialTransition getEndCondition(){
        return _endCondition;
    }
};

#endif /* TutorialStepData_hpp */
