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
#include "TutorialEvent.hpp"
#include "MoveEvent.hpp"

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
                    {
                        checkTransitionCondition(TutorialTransition::ON_PATH_DRAWN);
                        break;
                    }
                    case PathEvent::PathEventType::DRAWING:
                    {
                        checkTransitionCondition(TutorialTransition::ON_PATH_START);
                    }
                    default:
                    {
                        break;
                    }
                }
                break;
            }
        case Event::EventType::MOVE: {
            MoveEvent* moveEvent = (MoveEvent*)e;
            switch(moveEvent->_moveEventType){
                case MoveEvent::MoveEventType::MOVE_FINISHED:
                {
                    checkTransitionCondition(TutorialTransition::ON_MOVE_FINISHED);
                }
            }
            break;
        }
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWNING: {
                    ObjectSpawnEvent* objSpawnEvent = (ObjectSpawnEvent*)levelEvent;
                    std::shared_ptr<GameObject> obj = objSpawnEvent->object;
                    if (!obj->getIsPlayer() && obj->type == GameObject::ObjectType::CHARACTER){
                        checkTransitionCondition(TutorialTransition::ON_ENEMY_SPAWNING);
                    }
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objSpawnEvent = (ObjectSpawnEvent*)levelEvent;
                    std::shared_ptr<GameObject> obj = objSpawnEvent->object;
                    if (!obj->getIsPlayer() && obj->type == GameObject::ObjectType::CHARACTER){
                        checkTransitionCondition(TutorialTransition::ON_ENEMY_SPAWN);
                    }
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
                    ObjectHitEvent* ohEvent = (ObjectHitEvent*)e;
                    if (ohEvent->_obj->getIsPlayer()){
                        checkTransitionCondition(TutorialTransition::ON_PLAYER_HIT);
                    }
                    
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

///////////////////////////////Check_conditions/////////////////////////////////////

    /** this is to update other transition Condition that get checked on the spot */
    if (InputController::getIsPressed()){
        checkTransitionCondition(TutorialTransition::ON_CLICK);
    }
    
    /** check any immediate condition to go off */
    checkTransitionCondition(TutorialTransition::IMMEDIATE);
    
////////////////////////////////////////////////////////////////////////////////////
    
    /** check step is preactive and play its effect */
    if (getCurrentStep() != nullptr && getCurrentStep()->getState() == TutorialState::PRE_ACTIVE){
        /** play the start effects */
        handleTutorialEffects(getCurrentStep()->getStepData()->getStartEffects());
        getCurrentStep()->setState(TutorialState::ACTIVE);
    }
    
    /** remove all TutorialState::DONE hints */
    removeFinishedHints(_activeHints);
    
    /** remove all children of the tutorialNode */
    _tutorialNode->removeAllChildren();
    
    /** need to now add to tutorialNode the currentlyActiveHints and activeSteps*/
    for(std::shared_ptr<TutorialStep> hints : _activeHints){
        if (hints->isActive()){
            hints->addToNode(_tutorialNode);
        }
    }
    
    if (getCurrentStep() == nullptr){
        return;
    }
    
    /** add the current step if active */
    if (getCurrentStep() != nullptr && getCurrentStep()->isActive()){
        getCurrentStep()->addToNode(_tutorialNode);
    }
    
    /** update the currentStep if the condition has been met */
    /** updates at the end of update for next iteration */
    transitionNextStep();
}

bool TutorialController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> levelWorld) {
    _tutorialNode = Node::alloc();
    _currentStep = 0;
    // TODO replace with getting it from the current level that is active
    // or passed from the saveGameData file
    populateFromTutorial(levelWorld->getAssetManager(), levelWorld->getLevelData()->key);
    
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
    
    // there is no tutorial for this level
    if (tutData == nullptr){
        return;
    }
    
    for (std::string stepKey : tutData->getStepKeys()){
        // create the tutorialStep from the stepData
        std::shared_ptr<TutorialStepData> stepData = assets->get<TutorialStepData>(stepKey);
        std::shared_ptr<Menu> screen = Menu::alloc(stepData->key);
        
        screen->populate(assets,stepData->getUIEntryKeys(),stepData->menuBackgroundKey,tutData->getFontMap());
        
        std::shared_ptr<TutorialStep> step = TutorialStep::alloc(stepData);
        step->setMenu(screen);
        _steps.push_back(step);
    }
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
    
    /** play the post effects of the current step */
    handleTutorialEffects(getCurrentStep()->getStepData()->getEndEffects());
    
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
    
    return (isStepsEmpty) || (isStepsDone && isHintsDone);
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

void TutorialController::handleTutorialEffects(std::vector<TutorialEffect> effects){
    for (TutorialEffect e : effects) {
        handleTutorialEffect(e);
    }
}

void TutorialController::handleTutorialEffect(TutorialEffect effect){
    switch(effect){
        case TutorialEffect::PAUSE_SPAWNING:
        {
            std::shared_ptr<Event> pauseEvent = PauseSpawningEvent::alloc();
            notify(pauseEvent.get());
            break;
        }
        case TutorialEffect::RESUME_SPAWNING:
        {
            std::shared_ptr<Event> resumeEvent = ResumeSpawningEvent::alloc();
            notify(resumeEvent.get());
            break;
        }
        case TutorialEffect::NONE:
        {
            break;
        }
    }
}
