//
//  SwitchController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SwitchController.hpp"
#include "PathEvent.hpp"

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
	case Event::EventType::MOVE:
		SwitchController::switchFlag = true;
		break;
	default:
		break;
	}
}

void SwitchController::update(float timestep, std::shared_ptr<GameState> state) {
	if (switchFlag) {
//        state->getActiveCharacter()->getNode()->setColor(_baseColors[state->getActiveCharacter()]);
//        
		state->toggleActiveCharacter(); // new active character
//        
//        if (state->getActiveCharacter()->getPhysicsComponent()->getElementType() == Element::BLUE){
//            state->getActiveCharacter()->getNode()->setColor(cugl::Color4::BLUE); // tint active character;
//        } else {
//            state->getActiveCharacter()->getNode()->setColor(cugl::Color4::YELLOW); // tint active character;
//        }
        
        switchFlag = false;
	}
}

bool SwitchController::init(std::shared_ptr<GameState> state) {
	for (auto it : state->getPlayerCharacters()) {
		_baseColors[it] = it->getNode()->getColor();
	}
	return true;
}
