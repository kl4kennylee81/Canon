//
//  CollisionController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef CollisionController_hpp
#define CollisionController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_map>
#include <vector>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2Body.h>
#include "BaseController.hpp"
#include "GameObject.hpp"
#include "Observer.hpp"
#include "Event.hpp"

class CollisionController : public BaseController {
protected:
    /** The Box2D world */
    std::shared_ptr<cugl::ObstacleWorld> _world;

public:

	virtual void attach(std::shared_ptr<Observer> obs);
	virtual void detach(std::shared_ptr<Observer> obs);
	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);

	virtual bool init();

	virtual bool init(const cugl::Rect& bounds) {
		_world = cugl::ObstacleWorld::alloc(bounds);
		return true;

	}

	static std::shared_ptr<CollisionController> alloc() {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init() ? result : nullptr);
	}

	static std::shared_ptr<CollisionController> alloc(const cugl::Rect& bounds) {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init(bounds) ? result : nullptr);
	}
};

#endif /* CollisionController_hpp */
