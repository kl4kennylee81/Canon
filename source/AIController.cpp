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
		case LevelEvent::LevelEventType::OBJECT_SPAWN:
			ObjectSpawnEvent* spawn = (ObjectSpawnEvent*)levelEvent;
			auto ai = HomingAI::alloc(spawn->object);
			addAI(ai);
			break;
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
	
	//for (auto it : state->getEnemyObjects()) {
	//	Element e = it->getPhysicsComponent()->getElementType();
 //       
 //       // HACK we should check the actual active player list
 //       // and then iterate through to find closest one
 //       // no assumption on only 2
	//	int playerIndex = e == Element::BLUE ? 0 : 1;

 //       // HACK MUST FIX to keep from null pointering
 //       if (state->getPlayerCharacters().size() == 2){
 //           auto player = state->getPlayerCharacters().at(playerIndex);
 //           Vec2 playerPos = player->getNode()->getPosition();
 //           Vec2 enemyPos = it->getNode()->getPosition();
 //           Vec2 direction = MoveController::getVelocityVector(enemyPos, playerPos, AI_SPEED);
 //           it->getPhysicsComponent()->getBody()->setLinearVelocity(direction);
 //       }
	//}
}

bool AIController::init() {
	return true;
}
