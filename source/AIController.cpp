//
//  AIController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AIController.hpp"
#include "MoveController.hpp"
#include "Element.hpp"
#include "PathParameters.h"
#include "HomingAI.hpp"
#include "LevelEvent.hpp"
#include "CollisionEvent.hpp"

using namespace cugl;

AIController::AIController():
BaseController(){}

void AIController::attach(std::shared_ptr<Observer> obs) {
	BaseController::attach(obs);
}
void AIController::detach(Observer* obs) {
	BaseController::detach(obs);
}
void AIController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void AIController::eventUpdate(Event* e){
	switch (e->_eventType) {
	case Event::EventType::LEVEL: 
	{
		LevelEvent* levelEvent = (LevelEvent*)e;
		switch (levelEvent->levelEventType) {
            case LevelEvent::LevelEventType::OBJECT_INIT:
            {
                ObjectInitEvent* init = (ObjectInitEvent*)levelEvent;
                std::shared_ptr<HomingAI> ai = HomingAI::alloc(init->object);
                ai->getObjects();
                addAI(ai);
                break;
            }
            case LevelEvent::LevelEventType::OBJECT_SPAWN:
            {
                ObjectSpawnEvent* spawn = (ObjectSpawnEvent*)levelEvent;
                std::shared_ptr<ActiveAI> activeAi = _map.at(spawn->object.get());
                activeAi->toggleActive();
            }
		}
        
		break;
	}
	case Event::EventType::COLLISION: 
	{
		CollisionEvent* collisionEvent = (CollisionEvent*)e;
		switch (collisionEvent->_collisionType) {
		case CollisionEvent::CollisionEventType::OBJECT_GONE:
			ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
			removeAI(objectGone->_obj);
			break;
		}
		break;
	}
	}
}

void AIController::addAI(std::shared_ptr<ActiveAI> ai) {
	_enemies.insert(ai);
    ai->getObjects();
	for (auto it : ai->getObjects()) {
		_map[it.get()] = ai;
	}
}

void AIController::removeAI(GameObject* obj) {
	auto ai = _map[obj];
	if (ai->garbageCollect(obj)) {
		_enemies.erase(ai);
	}
	_map.erase(obj);
}

void AIController::update(float timestep,std::shared_ptr<GameState> state){
	for (auto it : _enemies) {
		it->update(state);
	}
}

bool AIController::init() {
	return true;
}
