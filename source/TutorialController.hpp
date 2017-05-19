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
#include "TutorialEnums.hpp"
#include "World.hpp"
#include "ActiveHandMovement.hpp"

class TutorialController : public BaseController {
private:
    std::shared_ptr<cugl::Node> _tutorialNode;
    
    std::vector<std::shared_ptr<TutorialStep>> _steps;
    
    /** these are all waiting and/or active and are removed when endCondition is met 
     *  loop through all active hints and see if they are met */
    std::list<std::shared_ptr<TutorialStep>> _activeHints;
    
    /** list of active hand components that are actively moving onscreen */
    std::list<std::shared_ptr<ActiveHandMovement>> _activeHandMovement;
    
    int _currentStep;
    
    bool _isActive; //tutorial controller is not active if a reset had happened
    
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
    
    virtual bool init(std::shared_ptr<GameState> state,
                      std::shared_ptr<World> levelWorld);
    
    void populateFromTutorial(std::shared_ptr<GenericAssetManager> assets,std::string tutorialKey);
    
    static std::shared_ptr<TutorialController> alloc(std::shared_ptr<GameState> state,
                                                     std::shared_ptr<World> levelWorld){
        std::shared_ptr<TutorialController> result = std::make_shared<TutorialController>();
        return (result->init(state,levelWorld) ? result : nullptr);
    }
    
    std::shared_ptr<TutorialStep> getCurrentStep();
    
    /** triggers the post effects */
    void transitionNextStep(std::shared_ptr<GameState> state);
    
    bool isInActive();
    
    void checkTransitionCondition(TutorialTransition transition);
    
    void handleTutorialEffects(std::shared_ptr<GameState> state, std::vector<TutorialEffect> effects);
    
    void handleTutorialEffect(std::shared_ptr<GameState> state, TutorialEffect effect);
    
    void updateConditions(std::shared_ptr<GameState> state);
    
    void updateHint(std::shared_ptr<GameState> state);
    
    void updateStep(std::shared_ptr<GameState> state);
    
    void updateHandMovement(std::shared_ptr<GameState> state);
    
    /** update when step first becomes active */
    void updateStartStep(std::shared_ptr<GameState> state, std::shared_ptr<TutorialStep> step);
    
    /** update when step ends */
    void updateEndStep(std::shared_ptr<GameState> state, std::shared_ptr<TutorialStep> step);
    
    void removeSteps(std::list<std::shared_ptr<TutorialStep>> hints);
    
    void toggleActive();
};


#endif /* TutorialController_hpp */
