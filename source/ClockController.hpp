//
//  ClockController.hpp
//  Canon
//
//  Created by Hong Jeon on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ClockController_hpp
#define ClockController_hpp

#include <stdio.h>
#include "BaseController.hpp"

/**
 * The ClockController is the only controller in charge of updating the InternalClock in the GameState
 * according to the events that it recieves from other controllers.
 */
class ClockController : public BaseController {
public:
    ClockController();
    
    ~ClockController() { dispose(); };
    
    void dispose();
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);
    
    virtual bool init();
    
    static std::shared_ptr<ClockController> alloc() {
        std::shared_ptr<ClockController> result = std::make_shared<ClockController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* ClockController_hpp */
