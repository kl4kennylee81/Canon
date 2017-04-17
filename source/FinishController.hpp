//
//  FinishController.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef FinishController_hpp
#define FinishController_hpp

#include <stdio.h>
#include "BaseController.hpp"
#include "GameState.hpp"
#include "Event.hpp"

class FinishController : public BaseController {
private:
    bool wavesFinished;
    bool playerDead;
    bool notified;
    
public:
    FinishController();
    
    ~FinishController(){};
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep, std::shared_ptr<GameState> state);
    
    virtual bool init(std::shared_ptr<GameState> state);
    
    static std::shared_ptr<FinishController> alloc(std::shared_ptr<GameState> state) {
        std::shared_ptr<FinishController> result = std::make_shared<FinishController>();
        return (result->init(state) ? result : nullptr);
    }
};

#endif /* FinishController_hpp */
