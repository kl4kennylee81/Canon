//
//  ActiveParticleState.hpp
//  Canon
//
//  Created by Jonathan Chen on 5/13/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActiveParticleState_hpp
#define ActiveParticleState_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "GameState.hpp"
#include "ParticleStateData.hpp"

class ActiveParticleState {
private:
    
    std::shared_ptr<ParticleStateData> _data;
    
    AnimationAction currentAction;
    
public:
    
    bool init() {
        currentAction = AnimationAction::DEFAULT;
        return true;
    }
    
    static std::shared_ptr<ActiveParticleState> alloc() {
        std::shared_ptr<ActiveParticleState> result = std::make_shared<ActiveParticleState>();
        return (result->init() ? result : nullptr);
    }
    
    void setParticleStateData(std::shared_ptr<ParticleStateData> psd){
        _data = psd;
    }
    
    void setAnimationAction(AnimationAction aa){
        currentAction = aa;
    }
    
    std::vector<ParticleAction> getCurrentParticleActions(){
        if (currentAction == AnimationAction::DEFAULT){
            return std::vector<ParticleAction>();
        }
        return _data->getParticleActions(currentAction);
    }
    
    bool hasAnimationAction(AnimationAction aa){ return _data->hasAnimationAction(aa); }
    
};
#endif /* ActiveParticleState_hpp */
