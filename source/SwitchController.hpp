//
//  SwitchController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SwitchController_hpp
#define SwitchController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"

class SwitchController : public BaseController {
public:
    SwitchController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init();
    
    static std::shared_ptr<SwitchController> alloc() {
        std::shared_ptr<SwitchController> result = std::make_shared<SwitchController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* SwitchController_hpp */
