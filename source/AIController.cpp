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
#include "PathAI.hpp"
#include "StaticAI.hpp"

using namespace cugl;

AIController::AIController():
BaseController(){}

void AIController::attach(Observer* obs) {
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
                
                // the player character does not have an AIData
                if (init->aiData == nullptr){
                    return;
                }
				addAI(init->aiData->newActiveAI(init->object));
                break;
            }
            case LevelEvent::LevelEventType::OBJECT_SPAWN:
            {
                ObjectSpawnEvent* spawn = (ObjectSpawnEvent*)levelEvent;
                
                // check existence in hashmap if not init then it was likely the player character
                if (_map.count(spawn->object.get()) <= 0) {
                    return;
                }
                
                std::shared_ptr<ActiveAI> activeAi = _map.at(spawn->object.get());
                activeAi->toggleActive();
                break;
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
    // duplicate removeAI events are called there are cases where the AI is already removed
    if (_map.find(obj) == _map.end()) {
        return;
    }
    auto ai = _map.at(obj);
	if (ai->garbageCollect(obj)) {
		_enemies.erase(ai);
	}
	_map.erase(obj);
    
}

void AIController::update(float timestep,std::shared_ptr<GameState> state){
	for (auto it : _enemies) {
        if (it->isActive()){
            it->update(state);
        }
	}
}

bool AIController::init() {
	return true;
}
