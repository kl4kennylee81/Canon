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
        DRAWING,
        
        CANCELLED
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
    
    std::shared_ptr<Path> _path;
    ElementType _active_element;
    cugl::Vec2 _starting_point;
    
    bool init(std::shared_ptr<Path> path, ElementType element, cugl::Vec2 starting_point) {
        PathEvent::init();
        _pathType = PathEventType::DRAWING;
        _path = path;
        _active_element = element;
        _starting_point = starting_point;
        return true;
    }
    
    static std::shared_ptr<PathDrawing> alloc(std::shared_ptr<Path> path, ElementType element, cugl::Vec2 starting_point) {
        std::shared_ptr<PathDrawing> result = std::make_shared<PathDrawing>();
        return (result->init(path, element, starting_point) ? result : nullptr);
    }
};

class PathCancelled : public PathEvent {
public:
    PathCancelled() : PathEvent(){}
    
    bool init() {
        PathEvent::init();
        _pathType = PathEventType::CANCELLED;
        return true;
    }
    
    static std::shared_ptr<PathCancelled> alloc() {
        std::shared_ptr<PathCancelled> result = std::make_shared<PathCancelled>();
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
