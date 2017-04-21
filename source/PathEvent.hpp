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
        PATH_FINISHED,
        
        /** User is currently drawing */
        DRAWING
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

class PathDrawing : public PathEvent {
public:
    PathDrawing() : PathEvent(){}
    
    bool init() {
        PathEvent::init();
        _pathType = PathEventType::DRAWING;
        return true;
    }
    
    static std::shared_ptr<PathDrawing> alloc() {
        std::shared_ptr<PathDrawing> result = std::make_shared<PathDrawing>();
        return (result->init() ? result : nullptr);
    }
};

class PathFinished : public PathEvent {
public:
    std::shared_ptr<Path> _path;
    std::shared_ptr<GameObject> _activeChar;
    std::shared_ptr<GameObject> _inactiveChar;

	bool init(std::shared_ptr<Path> path, std::shared_ptr<GameObject> active,std::shared_ptr<GameObject> inactive) {
		PathEvent::init();
		_path = path->clone();
        this->_activeChar = active;
        this->_inactiveChar = inactive;
		_pathType = PathEventType::PATH_FINISHED;
		return true;
	}

	static std::shared_ptr<PathFinished> alloc(std::shared_ptr<Path> path, std::shared_ptr<GameObject> active,std::shared_ptr<GameObject> inactive) {
		std::shared_ptr<PathFinished> result = std::make_shared<PathFinished>();
		return (result->init(path, active,inactive) ? result : nullptr);
	}

	PathFinished() : PathEvent(),_path(nullptr) {}
};


#endif /* PathEvent_hpp */
