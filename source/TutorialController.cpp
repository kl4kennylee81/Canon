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
    switch (e->_eventType) {
            case Event::EventType::PATH:
            { // scoped brace for pe variable
            	PathEvent* pe = (PathEvent*)e;
            	switch (pe->_pathType) {
            	case PathEvent::PathEventType::PATH_FINISHED:
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
        default:
            break;
    }
}

void TutorialController::update(float timestep, std::shared_ptr<GameState> state) {
    
    
    
    
}

bool TutorialController::init(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets) {
    _tutorialNode = Node::alloc();
    _currentStep = 0;
    
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
    
    return true;
}

void TutorialController::populate(std::shared_ptr<GenericAssetManager> assets){
    return;
}
