//
//  SwitchController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SwitchController.hpp"
#include "PathEvent.hpp"
#include "SwitchEvent.hpp"

using namespace cugl;

SwitchController::SwitchController() :
	BaseController() {}

void SwitchController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}
void SwitchController::detach(Observer* obs) {
	BaseController::detach(obs);
}
void SwitchController::notify(Event* e) {
	BaseController::notify(e);
}

void SwitchController::eventUpdate(Event* e) {
	switch (e->_eventType) {
	//case Event::EventType::PATH:
	//{ // scoped brace for pe variable
	//	PathEvent* pe = (PathEvent*)e;
	//	switch (pe->_pathType) {
	//	case PathEvent::PathEventType::PATH_FINISHED:
	//		SwitchController::switchFlag = true;
	//		break;
	//	default:
	//		break;
	//	}

	//	break;
	//}
        case Event::EventType::MOVE: {
            SwitchController::switchFlag = true;
            break;
        }
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    if (spawnSwitch) {
                        switchFlag = true;
                        spawnSwitch = false;
                    }
                    break;
                }
            }
            break;
        }
	default:
		break;
	}
}

void SwitchController::update(float timestep, std::shared_ptr<GameState> state) {
	if (switchFlag) {
        
        if (state->getActiveCharacter() == nullptr){
            return;
        }
		state->toggleActiveCharacter(); // new active character
        std::shared_ptr<SwitchEvent> switchevent = SwitchEvent::alloc(state->getActiveCharacter());
        notify(switchevent.get());
        switchFlag = false;
	}
}

bool SwitchController::init(std::shared_ptr<GameState> state) {
    switchFlag = false;
    spawnSwitch = true;
	return true;
}
