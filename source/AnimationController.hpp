//
//  AnimationController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AnimationController_hpp
#define AnimationController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"
#include "ActiveAnimation.hpp"
#include "AnimationAction.hpp"
#include "WaveData.hpp"
#include <map>

class AnimationController : public BaseController {
protected:
    std::shared_ptr<cugl::Node> _worldnode;
    
    std::map<GameObject*, std::shared_ptr<ActiveAnimation>> animationMap;
    
public:
    AnimationController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets);
    
    static std::shared_ptr<AnimationController> alloc(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
        std::shared_ptr<AnimationController> result = std::make_shared<AnimationController>();
        return (result->init(state, assets) ? result : nullptr);
    }
    
    void addAnimation(GameObject* obj, std::shared_ptr<AnimationData> data);
    
    void handleAction(GameObject* obj, AnimationAction action);
    
    void syncAll();
    
    void syncAnimation(std::shared_ptr<cugl::AnimationNode> anim, GameObject* obj);
    
    void updateFrames();
};

#endif /* AnimationController_hpp */
