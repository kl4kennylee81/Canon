//
//  SoundData.hpp
//  Canon
//
//  Created by Jonathan Chen on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SoundData_hpp
#define SoundData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "Data.hpp"
#include "SoundAction.hpp"

class SoundData : public Data {
public:
    std::map<SoundAction, std::string> soundMap;
    
    SoundData(): Data(){};
    
    bool init();
    
    static std::shared_ptr<SoundData> alloc() {
        std::shared_ptr<SoundData> result = std::make_shared<SoundData>();
        return (result->init() ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    static SoundAction stringToAction(std::string action){
        if (action == "SLOW_MOTION_START") return SoundAction::SLOW_MOTION_START;
        if (action == "SLOW_MOTION_END") return SoundAction::SLOW_MOTION_END;
        if (action == "SPAWN") return SoundAction::SPAWN;
        if (action == "ACTIVE") return SoundAction::ACTIVE;
        if (action == "DEATH") return SoundAction::DEATH;
        if (action == "HIT") return SoundAction::HIT;
        std::cout << "stringToAction: SoundData: default action?\n";
        return SoundAction::DEFAULT;
    }
    
    static std::string actionToString(SoundAction action)
    {
        if (action == SoundAction::SLOW_MOTION_START) return "SLOW_MOTION_START";
        if (action == SoundAction::SLOW_MOTION_END) return "SLOW_MOTION_END";
        if (action == SoundAction::SPAWN) return "SPAWN";
        if (action == SoundAction::ACTIVE) return "ACTIVE";
        if (action == SoundAction::DEATH) return "DEATH";
        if (action == SoundAction::HIT) return "HIT";
        std::cout << "actionToString: SoundData: default action?\n";
        return "DEFAULT";
    }
    
};

#endif /* SoundData_hpp */
