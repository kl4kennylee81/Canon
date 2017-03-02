//
//  LevelController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelController_hpp
#define LevelController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"

class LevelController : BaseController {
public:
    LevelController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init();
    
    static std::shared_ptr<LevelController> alloc() {
        std::shared_ptr<LevelController> result = std::make_shared<LevelController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* LevelController_hpp */
