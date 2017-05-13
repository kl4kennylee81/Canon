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
#include "GenericAssetManager.hpp"
#include "GameState.hpp"
#include "Event.hpp"

enum class TutorialState : int {
    ACTIVE, // when it's waiting for an end transition condition
    WAITING, // inactive is when it is waiting for the start transition condition
    OFF, // this is not a tutorial level
    DONE
};

class TutorialController : public BaseController {
private:
    TutorialState _state;
    
    std::shared_ptr<cugl::Node> _tutorialNode;
    
    std::vector<std::shared_ptr<TutorialStep>> _steps;
    
    int _currentStep;
    
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
    
    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets);
    
    void populate(std::shared_ptr<GenericAssetManager> assets);
    
    static std::shared_ptr<TutorialController> alloc(std::shared_ptr<GameState> state,std::shared_ptr<GenericAssetManager> assets) {
        std::shared_ptr<TutorialController> result = std::make_shared<TutorialController>();
        return (result->init(state,assets) ? result : nullptr);
    }
    
    std::shared_ptr<TutorialStep> getCurrentStep();
    
    void transitionNextStep();
    
    void transitionToActive();
    
    bool isInActive();
};


#endif /* TutorialController_hpp */
