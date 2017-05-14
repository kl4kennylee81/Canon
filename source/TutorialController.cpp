//
//  TutorialController.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialController.hpp"
#include "TutorialLevelData.hpp"
#include "TutorialStepData.hpp"
#include "PathEvent.hpp"
#include "SwitchEvent.hpp"
#include "UIData.hpp"
#include "InputController.hpp"
#include "Menu.hpp"
#include "CollisionEvent.hpp"

using namespace cugl;

TutorialController::TutorialController() :
BaseController(),
_tutorialNode(nullptr),
_currentStep(0){}

void TutorialController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void TutorialController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void TutorialController::notify(Event* e) {
    BaseController::notify(e);
}

void TutorialController::eventUpdate(Event* e) {
    if (isInActive()){
        return;
    }
    
    switch (e->_eventType) {
            case Event::EventType::PATH:
            { // scoped brace for pe variable
            	PathEvent* pe = (PathEvent*)e;
            	switch (pe->_pathType) {
            	case PathEvent::PathEventType::PATH_FINISHED:
                        checkTransitionCondition(TutorialTransition::ON_PATH_DRAWN);
            		break;
            	default:
            		break;
            	}
            
            	break;
            }
        case Event::EventType::MOVE: {
            break;
        }
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    break;
                }
            }
            break;
        }
        case Event::EventType::COLLISION: {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType){
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                {
                    ObjectGoneEvent* ogEvent = (ObjectGoneEvent*)e;
                    if (!ogEvent->_obj->getIsPlayer()){
                        checkTransitionCondition(TutorialTransition::ON_ENEMY_CLEARED);
                    }
                    break;
                }
                case CollisionEvent::CollisionEventType::OBJECT_HIT:
                {
                    break;
                }
                case CollisionEvent::CollisionEventType::GAME_LOST:
                {
                    break;
                }
                case CollisionEvent::CollisionEventType::HIT_FINISHED:
                {
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void removeFinishedHints(std::list<std::shared_ptr<TutorialStep>> items){
    std::list<std::shared_ptr<TutorialStep>>::iterator i = items.begin();
    while (i != items.end())
    {
        bool isActive = (*i)->isDone();
        if (!isActive)
        {
            i = items.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void TutorialController::update(float timestep, std::shared_ptr<GameState> state) {
    // skip if tutorial isn't active
    if (isInActive()){
        return;
    }
    
    /** this is to update other transition Condition that get checked on the spot */
    if (InputController::getIsPressed()){
        checkTransitionCondition(TutorialTransition::ON_CLICK);
    }
    
    /** update the currentStep if the condition has been met */
    transitionNextStep();
    
    /** remove all TutorialState::DONE hints */
    removeFinishedHints(_activeHints);
    
    /** remove all children of the tutorialNode */
    _tutorialNode->removeAllChildren();
    
    /** need to now add to tutorialNode the currentlyActiveHints and activeSteps*/
    for(std::shared_ptr<TutorialStep> hints : _activeHints){
        if (hints->getState() == TutorialState::ACTIVE){
            hints->addToNode(_tutorialNode);
        }
    }
    
    if (getCurrentStep() == nullptr){
        return;
    }
    
    /** add the current step if active */
    if (getCurrentStep()->getState() == TutorialState::ACTIVE){
        getCurrentStep()->addToNode(_tutorialNode);
    }
}

bool TutorialController::init(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets) {
    _tutorialNode = Node::alloc();
    _currentStep = 0;
    // TODO replace with getting it from the current level that is active
    // or passed from the saveGameData file
    populateFromTutorial(assets,"tutorial1");
    
    if (_steps.size() > 0){
        // start the initial step as waiting
        getCurrentStep()->setState(TutorialState::WAITING);
    }
    
    // TODO 5 is a magic number just to get it to the front of the screen right now
    state->getScene()->addChild(_tutorialNode,5);
    return true;
}

void TutorialController::populateFromTutorial(std::shared_ptr<GenericAssetManager> assets,std::string tutorialKey){
    std::shared_ptr<TutorialLevelData> tutData = assets->get<TutorialLevelData>(tutorialKey);
    for (std::string stepKey : tutData->getStepKeys()){
        // create the tutorialStep from the stepData
        std::shared_ptr<TutorialStepData> stepData = assets->get<TutorialStepData>(stepKey);
        std::shared_ptr<Menu> screen = Menu::alloc(stepData->key);
        screen->populate(assets,stepData->getUIEntryKeys(),stepData->menuBackgroundKey);
        
        std::shared_ptr<TutorialStep> step = TutorialStep::alloc();
        step->setTransition(stepData->getStartCondition(),stepData->getEndCondition());
        step->setMenu(screen);
        _steps.push_back(step);
    }
}

void TutorialController::populate(std::shared_ptr<GenericAssetManager> assets){
    // make label for level entry
    std::shared_ptr<UIData> labelText = assets->get<UIData>("levelLabelText");
    std::shared_ptr<TextUIData> textData = std::dynamic_pointer_cast<TextUIData>(labelText);
    textData->textValue = "Tutorial Begins";
    std::shared_ptr<Node> labelNode = textData->dataToNode(assets);
    labelNode->setPosition(225,300);
    
    std::shared_ptr<TutorialStep> step = TutorialStep::alloc();
    std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(nullptr, labelNode);
    
    
    std::shared_ptr<Menu> tutorialScreen = Menu::alloc("tutorialStep1");
    
    tutorialScreen->addUIElement(uiComponent);
    
    step->setMenu(tutorialScreen);
    
    step->setTransition(TutorialTransition::ON_PATH_DRAWN,TutorialTransition::ON_ENEMY_CLEARED);
    
    _steps.push_back(step);
    
    labelText = assets->get<UIData>("levelLabelText");
    textData = std::dynamic_pointer_cast<TextUIData>(labelText);
    textData->textValue = "Tutorial Ends";
    labelNode = textData->dataToNode(assets);
    labelNode->setPosition(500,300);
    
    step = TutorialStep::alloc();
    uiComponent = UIComponent::alloc(nullptr, labelNode);
    
    tutorialScreen = Menu::alloc("tutorialStep2");
    
    tutorialScreen->addUIElement(uiComponent);
    
    step->setMenu(tutorialScreen);
    
    step->setTransition(TutorialTransition::ON_CLICK,TutorialTransition::ON_ENEMY_CLEARED);
    
    _steps.push_back(step);
    return;
}

std::shared_ptr<TutorialStep> TutorialController::getCurrentStep(){
    if (_currentStep >= _steps.size()){
        return nullptr;
    }
    return _steps.at(_currentStep);
}

void TutorialController::transitionNextStep(){
    if (getCurrentStep() == nullptr){
        return;
    }
    
    if (getCurrentStep()->getState() != TutorialState::DONE){
        return;
    }
    
    /** TODO also add all the hints from this step into the activeHints */
    
    _currentStep+=1;
    
    /** end of the steps */
    if (_currentStep >= _steps.size()){
        return;
    }
    
    /** set the next step to waiting */
    getCurrentStep()->setState(TutorialState::WAITING);
    return;
}

bool TutorialController::isInActive(){
    bool isStepsEmpty = _steps.size() == 0;
    bool isStepsDone = _currentStep >= _steps.size();
    bool isHintsDone = _activeHints.size() == 0;
    
    return isStepsEmpty && isStepsDone && isHintsDone;
}

void TutorialController::checkTransitionCondition(TutorialTransition transition){
    
    for (std::shared_ptr<TutorialStep> hint : _activeHints){
        // check if the start/end condition is the same as the given condition
        hint->checkCondition(transition);
    }
    
    // check current Step exists
    if (_currentStep >= _steps.size()) {
        return;
    }
    
    // update the state of the currentStep
    getCurrentStep()->checkCondition(transition);
}
