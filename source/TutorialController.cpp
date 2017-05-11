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

using namespace cugl;

TutorialController::TutorialController() :
BaseController() {}

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

bool TutorialController::init(std::shared_ptr<GameState> state) {
    return true;
}
