//
//  MoveController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MoveController.hpp"
#include "PathEvent.hpp"
#include "PathParameters.h"
#include "CollisionEvent.hpp"
#include "MoveEvent.hpp"

using namespace cugl;

MoveController::MoveController():
BaseController(){}

void MoveController::attach(Observer* obs) {
	BaseController::attach(obs);
}

void MoveController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void MoveController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void MoveController::eventUpdate(Event* e) {
	switch (e->_eventType) {
	case Event::EventType::PATH:
		PathEvent* pathEvent = (PathEvent*)e;
		switch (pathEvent->_pathType) {
		case PathEvent::PathEventType::PATH_FINISHED:
			PathFinished* pathFinished = (PathFinished*)pathEvent;
			std::shared_ptr<ActivePath> path = ActivePath::alloc(pathFinished->_path);
			_activePaths[pathFinished->_activeChar] = path;
			break;
		}
		break;
	}
}

void MoveController::update(float timestep,std::shared_ptr<GameState> state){
	updateActivePaths(timestep, state);
	std::vector<std::shared_ptr<GameObject>> toDelete;
	for (auto it : _activePaths) {
		std::shared_ptr<GameObject> player = it.first;
		std::shared_ptr<ActivePath> path = it.second;
        if (player == nullptr){
            return;
        }
		Vec2 goal = path->_path->get(path->_pathIndex);
		Vec2 current = player->getPosition();
        Vec2 velocity = getVelocityVector(current, goal,((float) VELOCITY * 60));
		player->getPhysicsComponent()->getBody()->setLinearVelocity(velocity);
	}
}

void MoveController::updateActivePaths(float timestep, std::shared_ptr<GameState> state) {
	std::vector<std::shared_ptr<GameObject>> toDelete;
	for (auto it : _activePaths) {
		std::shared_ptr<GameObject> player = it.first;
		std::shared_ptr<ActivePath> path = it.second;
        if (player == nullptr){
            return;
        }
		Vec2 goal = path->_path->get(path->_pathIndex);
		Vec2 current = player->getPosition();
		if (std::abs(current.distance(goal)) <= (((float) RADIUS))) {
			path->_pathIndex = path->_pathIndex + 1;
			if (path->_pathIndex >= path->_path->size()) {
				player->getPhysicsComponent()->getBody()->setVX(0);
				player->getPhysicsComponent()->getBody()->setVY(0);
				auto moveEvent = MoveFinished::alloc(player);
				notify(moveEvent.get());
				toDelete.push_back(player);
			}
		}
	}
    
    // TODO replace with iterator delete and break
	for (auto it : toDelete) {
		_activePaths.erase(it);
	}
}

bool MoveController::init(std::shared_ptr<GameState> state) {
	return true;
}

/**
 * Return velocity vector with current time dilation applied
 */
Vec2 MoveController::getVelocityVector(Vec2 start, Vec2 end, float velocity)
{
	Vec2 direction = Vec2::Vec2(end.x, end.y).subtract(start);
	direction.normalize().scale(velocity * GameState::_internalClock->getTimeDilation());
	return direction;
}

std::string MoveController::serialize() {
	return toJsonValue()->toString();
}

std::shared_ptr<JsonValue> MoveController::toJsonValue() {
	std::shared_ptr<JsonValue> fullJson = JsonValue::allocObject();

	for (auto entry : _activePaths) {
		int uid = entry.first->getUid();
		int pathIndex = entry.second->_pathIndex;
		std::shared_ptr<JsonValue> pathVecs = entry.second->_path->toJsonValue();

		std::shared_ptr<JsonValue> pathJson = JsonValue::allocObject();
		pathJson->appendChild("pathIndex", JsonValue::alloc(std::to_string(pathIndex)));
		pathJson->appendChild("pathVertices", pathVecs);
		
		fullJson->appendChild(std::to_string(uid), pathJson);

	}
	return fullJson;
}

void MoveController::initAfterResume(std::shared_ptr<GameState> state, std::shared_ptr<JsonValue> rJson) {
	//{
//		"1":	{
	//		"pathIndex":	"3",
	//			"pathVertices" : [[18.750000, 7.812500], [18.266668, 7.066667]]
	//	}
	//},

	for (int i = 0; i < rJson->size(); i++) {
		std::shared_ptr<JsonValue> obj = rJson->get(i);
		
		int uid = std::stoi(obj->key());
		int pathIndex = std::stoi(obj->getString("pathIndex"));

		std::shared_ptr<JsonValue> jv = obj->get("pathVertices");
		std::vector<Vec2> vecList;
		for (int j = 0; j < jv->size(); j++) {
			std::shared_ptr<JsonValue> vertex = jv->get(j);
			float x = vertex->getFloat("x");
			float y = vertex->getFloat("y");
			Vec2 temp = Vec2(x, y);
			vecList.push_back(temp);
		}

		std::shared_ptr<GameObject> gObj = state->getUID2GameObject(uid);
		_activePaths[gObj] = ActivePath::alloc(Path::alloc(vecList), pathIndex);

		//std::unordered_map <std::shared_ptr<GameObject>, std::shared_ptr<ActivePath>> _activePaths;

	}
}
