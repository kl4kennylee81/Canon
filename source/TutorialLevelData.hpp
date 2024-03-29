//
//  TutorialLevelData.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialLevelData_hpp
#define TutorialLevelData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include "Data.hpp"
#include <map>

class TutorialLevelData : public Data {
public:
    
    std::vector<std::string> _stepKeys;
    std::map<std::string,std::string> _fontMap;
    std::string _levelKey; // the levelKey that this tutorial serves
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    virtual bool preload(const std::string& file) override;
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    bool init(std::vector<std::string> sKey, std::string lKey,std::map<std::string,std::string> fontMap) {
        _fontMap = fontMap;
        _stepKeys = sKey;
        _levelKey = lKey;
        return true;
    }
    
    static std::shared_ptr<TutorialLevelData> alloc(std::vector<std::string> sKey, std::string lKey,
                                                    std::map<std::string,std::string> fontMap) {
        std::shared_ptr<TutorialLevelData> result = std::make_shared<TutorialLevelData>();
        return (result->init(sKey,lKey,fontMap) ? result : nullptr);
    }
    
    std::vector<std::string> getStepKeys(){
        return _stepKeys;
    }
    
    std::string getLevelKey(){
        return _levelKey;
    }
    
    std::map<std::string,std::string> getFontMap(){
        return _fontMap;
    }
};

#endif /* TutorialLevelData_hpp */
