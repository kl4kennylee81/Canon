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
#include "HandMovementComponent.hpp"

class TutorialStepData : public Data {
private:
    std::vector<std::string> _uiEntryKeys;
    std::vector<TutorialEffect> _startEffects;
    std::vector<TutorialEffect> _endEffects;
    float _minTime; // minimum time to stay on this step
    
    std::shared_ptr<HandMovementComponent> _handMovement;
    
public:
    
    std::string menuBackgroundKey; // replace the background for narrative start screens
    
    TutorialTransition _startCondition;
    TutorialTransition _endCondition;
    
    TutorialStepData():
    Data(),
    _minTime(0),
    _handMovement(nullptr){};
    
    ~TutorialStepData(){
        dispose();
    }
    
    void dispose(){
        _minTime = 0;
        _handMovement = nullptr;
    }
    
    std::vector<std::string> getUIEntryKeys() {
        return _uiEntryKeys;
    }
    
    bool init(std::string mbKey, std::vector<std::string> keys,
              TutorialTransition start,TutorialTransition end,
              std::vector<TutorialEffect> startEffects,
              std::vector<TutorialEffect> endEffects,
              float minTime) {
        menuBackgroundKey = mbKey;
        _uiEntryKeys = keys;
        _startCondition = start;
        _endCondition = end;
        _startEffects = startEffects;
        _endEffects = endEffects;
        _minTime = minTime;
        return true;
    }
    
    static std::shared_ptr<TutorialStepData> alloc(std::string mbKey, std::vector<std::string> keys,
                                                   TutorialTransition start,TutorialTransition end,
                                                   std::vector<TutorialEffect> startEffect,
                                                   std::vector<TutorialEffect> endEffect,
                                                   float minTime) {
        std::shared_ptr<TutorialStepData> result = std::make_shared<TutorialStepData>();
        return (result->init(mbKey, keys,start,end,startEffect,endEffect,minTime) ? result : nullptr);
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
    
    std::vector<TutorialEffect> getStartEffects(){
        return _startEffects;
    }
    
    std::vector<TutorialEffect> getEndEffects(){
        return _endEffects;
    }
    
    float getMinTime(){
        return _minTime;
    }
    
    std::shared_ptr<HandMovementComponent> getHandMovementComponent(){
        return _handMovement;
    }
};

#endif /* TutorialStepData_hpp */
