//
//  PathEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MoveEvent_hpp
#define MoveEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/base/CUBase.h>
#include "Event.hpp"
#include "GameEngine.hpp"

class MoveEvent : public Event {
public:
	enum class MoveEventType : int {
		/** Signal that a path is done */
		MOVE_FINISHED
	};
    
    std::shared_ptr<GameObject> _character;

	bool init( std::shared_ptr<GameObject> c) {
		Event::init();
		_eventType = Event::EventType::MOVE;
        _character = c;
		return true;
	}

	static std::shared_ptr<MoveEvent> alloc( std::shared_ptr<GameObject> c) {
		std::shared_ptr<MoveEvent> result = std::make_shared<MoveEvent>();
		return (result->init(c) ? result : nullptr);
	}

	MoveEvent() : Event() {}
};


#endif /* MoveEvent_hpp */
