//
//  TutorialController.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TutorialController.hpp"
#include "PathEvent.hpp"
#include "SwitchEvent.hpp"
#include "UIData.hpp"
#include "InputController.hpp"
#include "CollisionEvent.hpp"

using namespace cugl;

TutorialController::TutorialController() :
BaseController(),
_tutorialNode(nullptr),
_currentStep(0),
_state(TutorialState::OFF){}

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
                        if (getCurrentStep()->getEndCondition() == TutorialTransition::ON_PATH_DRAWN){
                            transitionNextStep();
                            return;
                        }
                        else if (getCurrentStep()->getStartCondition() == TutorialTransition::ON_PATH_DRAWN){
                            transitionToActive();
                            return;
                        }
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
                    if (!ogEvent->_obj->getIsPlayer() && getCurrentStep()->getEndCondition() == TutorialTransition::ON_ENEMY_CLEARED){
                        transitionNextStep();
                        return;
                    }
                    
                    else if (!ogEvent->_obj->getIsPlayer() && getCurrentStep()->getStartCondition() == TutorialTransition::ON_ENEMY_CLEARED){
                        transitionToActive();
                        return;
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

void TutorialController::update(float timestep, std::shared_ptr<GameState> state) {
    // skip if tutorial isn't active
    if (isInActive()){
        return;
    }
    
    switch(_state){
        case TutorialState::DONE:
        {
            return;
        }
        case TutorialState::OFF:
        {
            return;
        }
        case TutorialState::WAITING:
        {
            switch(getCurrentStep()->getStartCondition()){
                case TutorialTransition::ON_CLICK:
                {
                    if (!InputController::getIsPressed()){
                        return;
                    }
                    
                    transitionToActive();
                    break;
                }
            }
            return;
        }
        case TutorialState::ACTIVE:
        {
            return;
        }
    }
    
    
    
}

bool TutorialController::init(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets) {
    _tutorialNode = Node::alloc();
    _currentStep = 0;
    _state = TutorialState::OFF;
    
    populate(assets);
    
    if (_steps.size() > 0){
        _state = TutorialState::WAITING;
    }
    // TODO 5 is a magic number just to get it to the front of the screen right now
    state->getScene()->addChild(_tutorialNode,5);
    return true;
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
    
    step->setUIComponent(uiComponent);
    step->setTransition(TutorialTransition::ON_PATH_DRAWN,TutorialTransition::ON_ENEMY_CLEARED);
    
    _steps.push_back(step);
    
    labelText = assets->get<UIData>("levelLabelText");
    textData = std::dynamic_pointer_cast<TextUIData>(labelText);
    textData->textValue = "Tutorial Ends";
    labelNode = textData->dataToNode(assets);
    labelNode->setPosition(500,300);
    
    step = TutorialStep::alloc();
    uiComponent = UIComponent::alloc(nullptr, labelNode);
    
    step->setUIComponent(uiComponent);
    step->setTransition(TutorialTransition::ON_CLICK,TutorialTransition::ON_ENEMY_CLEARED);
    
    _steps.push_back(step);
    return;
}

std::shared_ptr<TutorialStep> TutorialController::getCurrentStep(){
    return _steps.at(_currentStep);
}

void TutorialController::transitionNextStep(){
    if (_state != TutorialState::ACTIVE){
        return;
    }
    
    _currentStep+=1;
    _tutorialNode->setVisible(false);
    _tutorialNode->removeAllChildren();
    _state = TutorialState::WAITING;
    
    if (_currentStep >= _steps.size()){
        _state = TutorialState::DONE;
    }
    return;
}

void TutorialController::transitionToActive(){
    if (_state != TutorialState::WAITING){
        return;
    }
    _state = TutorialState::ACTIVE;
    _tutorialNode->setVisible(true);
    _tutorialNode->removeAllChildren();
    
    std::shared_ptr<UIComponent> ui = getCurrentStep()->getUIComponent();
    _tutorialNode->addChild(ui->getNode());
    return;
}

bool TutorialController::isInActive(){
    return _state == TutorialState::OFF || _state == TutorialState::DONE;
}
