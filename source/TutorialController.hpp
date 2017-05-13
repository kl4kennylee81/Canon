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
#include <list>
#include "TutorialStep.hpp"
#include "BaseController.hpp"
#include "GenericAssetManager.hpp"
#include "GameState.hpp"
#include "Event.hpp"

class TutorialController : public BaseController {
private:
    std::shared_ptr<cugl::Node> _tutorialNode;
    
    std::vector<std::shared_ptr<TutorialStep>> _steps;
    
    /** these are all waiting and/or active and are removed when endCondition is met 
     *  loop through all active hints and see if they are met */
    std::list<std::shared_ptr<TutorialStep>> _activeHints;
    
    int _currentStep;
    
public:
    TutorialController();
    
    ~TutorialController(){ dispose(); };
    
    void dispose(){
        if (_tutorialNode != nullptr){
            _tutorialNode->removeFromParent();
        }
        _tutorialNode = nullptr;
        _currentStep = 0;
    }
    
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
    
    bool isInActive();
    
    void checkTransitionCondition(TutorialTransition transition);
};


#endif /* TutorialController_hpp */
