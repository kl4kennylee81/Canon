//
//  MoveController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MoveController_hpp
#define MoveController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Observer.hpp"
#include "Event.hpp"

class MoveController : public BaseController {

public:
	virtual void attach(std::shared_ptr<Observer> obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);

	virtual bool init();

	static std::shared_ptr<MoveController> alloc() {
		std::shared_ptr<MoveController> result = std::make_shared<MoveController>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* MoveController_hpp */
