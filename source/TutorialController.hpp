//
//  TutorialController.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialController_hpp
#define TutorialController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <vector>
#include "TutorialStep.hpp"
#include "BaseController.hpp"
#include "GameState.hpp"
#include "Event.hpp"


class TutorialController : public BaseController {
private:
    std::vector<std::shared_ptr<TutorialStep>> _steps;
    
public:
    TutorialController();
    
    ~TutorialController(){};
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep, std::shared_ptr<GameState> state);
    
    virtual bool init(std::shared_ptr<GameState> state);
    
    void populate(std::shared_ptr<GenericAssetManager> assets);
    
    static std::shared_ptr<TutorialController> alloc(std::shared_ptr<GameState> state) {
        std::shared_ptr<TutorialController> result = std::make_shared<TutorialController>();
        return (result->init(state) ? result : nullptr);
    }
};


#endif /* TutorialController_hpp */
