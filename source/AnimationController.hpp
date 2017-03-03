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

class AnimationController : public BaseController {
protected:
    std::vector<GameObject*> objsToRemove;
    
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
    
    virtual bool init(std::shared_ptr<GameState> state);
    
    static std::shared_ptr<AnimationController> alloc(std::shared_ptr<GameState> state) {
        std::shared_ptr<AnimationController> result = std::make_shared<AnimationController>();
        return (result->init(state) ? result : nullptr);
    }
};

#endif /* AnimationController_hpp */
