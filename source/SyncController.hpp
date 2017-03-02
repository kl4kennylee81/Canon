//
//  SyncController.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/2/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SyncController_hpp
#define SyncController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"

class SyncController : public BaseController {
public:
    SyncController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init();
    
    static std::shared_ptr<SyncController> alloc() {
        std::shared_ptr<SyncController> result = std::make_shared<SyncController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* SyncController_hpp */
