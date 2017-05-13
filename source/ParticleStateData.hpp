//
//  ParticleStateData.hpp
//  Canon
//
//  Created by Jonathan Chen on 5/13/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleStateData_hpp
#define ParticleStateData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "Data.hpp"
#include "AnimationAction.hpp"
#include "ParticleAction.hpp"

class ParticleStateData : public Data {
public:
    std::map<AnimationAction, std::vector<ParticleAction>> stateMap;
    
    ParticleStateData(): Data(){};
    
    bool init();
    
    static std::shared_ptr<ParticleStateData> alloc() {
        std::shared_ptr<ParticleStateData> result = std::make_shared<ParticleStateData>();
        return (result->init() ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
  
    bool hasAnimationAction(AnimationAction aa);
    
    std::vector<ParticleAction> getParticleActions(AnimationAction aa);
    
};

#endif /* ParticleStateData_hpp */
