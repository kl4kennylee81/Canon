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
                        break;
                    }
                    case PathEvent::PathEventType::CANCELLED:
                    {
                        checkTransitionCondition(TutorialTransition::ON_PATH_CANCELED);
                        break;
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

void TutorialController::removeSteps(std::list<std::shared_ptr<TutorialStep>> steps){
    std::list<std::shared_ptr<TutorialStep>>::iterator i = steps.begin();
    while (i != steps.end())
    {
        std::shared_ptr<TutorialStep> step = *i;
        bool isDone = step->isDone();
        if (isDone)
        {
            i = steps.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

bool checkButtonClicked(std::shared_ptr<TutorialStep> step, bool* buttonExist) {
    for (std::shared_ptr<UIComponent> ui : step->getMenu()->getUIElements()){
        // check if its a button
        std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(ui->getNode());
        
        if (button == nullptr){
            continue;
        }
        
        *buttonExist = true;
        Vec2 vec = InputController::getPrevVector();
        
        // check if this button was clicked
        if (!button->containsScreen(vec)){
            continue;
        }
        return true;
    }
    if (!*buttonExist){
        return true;
    }
    return false;
}

void TutorialController::updateConditions(std::shared_ptr<GameState> state){
    /** this is to update other transition Condition that get checked on the spot */
    
    if (InputController::getIsPressed()){
        /** if there exist a button ui element we want to make sure that we clicked that button
          * to continue instead of just registering any click as a success */
        
        // check if there exists a button
        bool buttonExist = false;
        bool clicked = false;
        if (getCurrentStep() != nullptr){
            clicked = checkButtonClicked(getCurrentStep(),&buttonExist);
        }
        
        for (std::shared_ptr<TutorialStep> hint : _activeHints){
            clicked |= checkButtonClicked(hint, &buttonExist);
        }
        
        if (!buttonExist||clicked){
            checkTransitionCondition(TutorialTransition::ON_CLICK);
        }
    }
    
    /** check any immediate condition to go off */
    checkTransitionCondition(TutorialTransition::IMMEDIATE);
    
    /** check condition for enemy cleared if no enemies are in the state and it is state tutorial paused */
    if (state->getGameplayState() == GameplayState::TUTORIAL_PAUSE && state->getEnemyObjects().size() <= 0){
        checkTransitionCondition(TutorialTransition::ON_ENEMY_CLEARED);
    }
}

void TutorialController::updateHint(std::shared_ptr<GameState> state){
    /** need to now add to tutorialNode the currentlyActiveHints and activeSteps*/
    for(std::shared_ptr<TutorialStep> hint : _activeHints){
        // update the hints minTime
        hint->update();
        
        /** check step is preactive and play its effect */
        if (hint->getState() == TutorialState::PRE_ACTIVE){
            updateStartStep(state,hint);
        }
        
        if (hint->isActive()){
            hint->addToNode(_tutorialNode);
        }
        else if (hint->isDone()){
            updateEndStep(state,hint);
        }
    }
    
    /** remove all TutorialState::DONE hints */
    removeSteps(_activeHints);
}

void TutorialController::updateHandMovement(std::shared_ptr<GameState> state){
    for (std::shared_ptr<ActiveHandMovement> activeH : _activeHandMovement){
        // returns true if path is done
        if (!activeH->update()){
            _tutorialNode->addChild(activeH->_node);
        }
    }
}

void TutorialController::updateStep(std::shared_ptr<GameState> state){
    /** update the current step */
    if (getCurrentStep() == nullptr){
        return;
    }
    
    /** update first */
    getCurrentStep()->update();
    
    /** check step is preactive and play its effect */
    if (getCurrentStep() != nullptr && getCurrentStep()->getState() == TutorialState::PRE_ACTIVE){
        updateStartStep(state, getCurrentStep());
    }
    
    /** add the current step if active */
    if (getCurrentStep() != nullptr && getCurrentStep()->isActive()){
        getCurrentStep()->addToNode(_tutorialNode);
    }
    
    /** update the currentStep if the condition has been met */
    /** updates at the end of update for next iteration */
    transitionNextStep(state);
}

/** update when step first becomes active */
void TutorialController::updateStartStep(std::shared_ptr<GameState> state, std::shared_ptr<TutorialStep> step){
    /** play the start effects */
    handleTutorialEffects(state, step->getStepData()->getStartEffects());
    step->setState(TutorialState::ACTIVE);
    
    if (step->getActiveHand() != nullptr){
        // add the active hand to the tutorial
        _activeHandMovement.push_back(step->getActiveHand());
    }
}

/** update when step ends */
void TutorialController::updateEndStep(std::shared_ptr<GameState> state, std::shared_ptr<TutorialStep> step){
    /** play the post effects of the current step */
    handleTutorialEffects(state, step->getStepData()->getEndEffects());
    
    /** clear the handmovement from this step since it has finished */
    if (step->getActiveHand() != nullptr){
        // set the handComponent node to not visible
        getCurrentStep()->getActiveHand()->_node->setVisible(false);
        _activeHandMovement.remove(getCurrentStep()->getActiveHand());
    }
    
    /** TODO also add all the hints from this step into the activeHints when step ends */
    for (std::shared_ptr<TutorialStep> hint : step->getHints()){
        hint->setState(TutorialState::WAITING);
        _activeHints.push_back(hint);
    }
}

void TutorialController::update(float timestep, std::shared_ptr<GameState> state) {
    // skip if tutorial isn't active
    if (isInActive()){
        return;
    }
    
    /** start off by clearing the tutorial Node */
    _tutorialNode->removeAllChildren();

    /** addes nodes back to tutorial node */
    updateHandMovement(state);
    updateStep(state);
    updateHint(state);
    updateConditions(state);
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
    state->getGameplayNode()->addChild(_tutorialNode,5);
    return true;
}

std::shared_ptr<TutorialStep> createTutorialStep(std::shared_ptr<GenericAssetManager> assets,
                                                 std::shared_ptr<TutorialStepData> stepData,
                                                 std::map<std::string,std::string> fontMap){
    std::shared_ptr<Menu> screen = Menu::alloc(stepData->key);
    
    screen->populate(assets,stepData->getUIEntryKeys(),stepData->menuBackgroundKey,fontMap);
    
    std::shared_ptr<TutorialStep> step = TutorialStep::alloc(stepData);
    step->setMenu(screen);
    
    /** create from handComponent active hand */
    if (stepData->getHandMovementComponent() != nullptr){
        std::shared_ptr<ActiveHandMovement> hand = ActiveHandMovement::alloc(assets, stepData->getHandMovementComponent(),fontMap);
        step->setActiveHand(hand);
    }
    return step;
}

void TutorialController::populateFromTutorial(std::shared_ptr<GenericAssetManager> assets,std::string tutorialKey){
    std::shared_ptr<TutorialLevelData> tutData = assets->get<TutorialLevelData>(tutorialKey);
    
    // there is no tutorial for this level
    if (tutData == nullptr){
        return;
    }
    
    for (std::string stepKey : tutData->getStepKeys()){
        std::shared_ptr<TutorialStepData> stepData = assets->get<TutorialStepData>(stepKey);
        std::shared_ptr<TutorialStep> step = createTutorialStep(assets,stepData,tutData->getFontMap());
        
        for (std::string hintKey : stepData->getHintKeys()){
            std::shared_ptr<TutorialStepData> hintData = assets->get<TutorialStepData>(hintKey);
            std::shared_ptr<TutorialStep> hint = createTutorialStep(assets,hintData,tutData->getFontMap());
            step->addHint(hint);
        }
        _steps.push_back(step);
    }
}

std::shared_ptr<TutorialStep> TutorialController::getCurrentStep(){
    if (_currentStep >= _steps.size()){
        return nullptr;
    }
    return _steps.at(_currentStep);
}

/** handle post condition of each tutorial step */
void TutorialController::transitionNextStep(std::shared_ptr<GameState> state){
    if (getCurrentStep() == nullptr){
        return;
    }
    
    if (getCurrentStep()->getState() != TutorialState::DONE){
        return;
    }
    
    updateEndStep(state, getCurrentStep());
    
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

void TutorialController::handleTutorialEffects(std::shared_ptr<GameState> state, std::vector<TutorialEffect> effects){
    for (TutorialEffect e : effects) {
        handleTutorialEffect(state, e);
    }
}

void TutorialController::handleTutorialEffect(std::shared_ptr<GameState> state, TutorialEffect effect){
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
        case TutorialEffect::PAUSE_GAME:
        {
            state->toggleTutorialPause();
            break;
        }
        case TutorialEffect::RESUME_GAME:
        {
            state->toggleTutorialPause();
            break;
        }
        case TutorialEffect::NONE:
        {
            break;
        }
    }
}
