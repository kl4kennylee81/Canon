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
#include "GameState.hpp"

class CollisionController : public BaseController {
protected:
    /** The Box2D world */
    std::shared_ptr<cugl::ObstacleWorld> _world;

public:
    
    CollisionController();

	virtual void attach(std::shared_ptr<Observer> obs);
	virtual void detach(Observer* obs);
	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);

	virtual bool init();

    virtual bool init(std::shared_ptr<GameState> state);

	static std::shared_ptr<CollisionController> alloc() {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init() ? result : nullptr);
	}

    static std::shared_ptr<CollisionController> alloc(std::shared_ptr<GameState> state) {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init(state) ? result : nullptr);
	}
    
    void beginContact(b2Contact* contact);
    
    void beforeSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif /* CollisionController_hpp */
