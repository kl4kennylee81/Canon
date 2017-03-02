//
//  MoveController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MoveController.hpp"
#include "PathEvent.hpp"

using namespace cugl;

MoveController::MoveController(){}

void MoveController::attach(std::shared_ptr<Observer> obs) {
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
			_activePaths[pathFinished->_character] = path;
			break;
		}
		break;
	}
}

void MoveController::update(float timestep,std::shared_ptr<GameState> state){
	std::vector<std::shared_ptr<GameObject>> toDelete;
	for (auto it : _activePaths) {
		std::shared_ptr<GameObject> player = it.first;
		std::shared_ptr<ActivePath> path = it.second;
		Vec2 vec = path->_path->get(path->_pathIndex);
		player->getNode()->setPosition(vec);
		path->_pathIndex = path->_pathIndex + 1;
		if (path->_pathIndex >= path->_path->size()) {
			toDelete.push_back(player);
		}
	}
	for (auto it : toDelete) {
		_activePaths.erase(it);
	}
}

bool MoveController::init(std::shared_ptr<GameState> state) {
	return true;
}
