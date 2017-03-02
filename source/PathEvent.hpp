//
//  PathEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathEvent_hpp
#define PathEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/base/CUBase.h>
#include "Path.hpp"
#include "Event.hpp"
#include "GameEngine.hpp"

class PathEvent : public Event {
public:
	enum class PathEventType : int {
        /** Signal that a path is done */
        PATH_FINISHED
    };

	PathEventType _pathType;

	bool init() {
		Event::init();
		_eventType = Event::EventType::PATH;
		return true;
	}

	static std::shared_ptr<PathEvent> alloc() {
		std::shared_ptr<PathEvent> result = std::make_shared<PathEvent>();
		return (result->init() ? result : nullptr);
	}

	PathEvent() : Event(){}
};

class PathFinished : public PathEvent {
public:
    std::shared_ptr<Path> _path;
	std::shared_ptr<GameObject> _character;

	bool init(std::shared_ptr<Path> path, std::shared_ptr<GameObject> character) {
		PathEvent::init();
		_path = path->clone();
		_character = character;
		_pathType = PathEventType::PATH_FINISHED;
		return true;
	}

	static std::shared_ptr<PathFinished> alloc(std::shared_ptr<Path> path, std::shared_ptr<GameObject> character) {
		std::shared_ptr<PathFinished> result = std::make_shared<PathFinished>();
		return (result->init(path, character) ? result : nullptr);
	}

	PathFinished() : PathEvent(),_path(nullptr) {}
};


#endif /* PathEvent_hpp */
