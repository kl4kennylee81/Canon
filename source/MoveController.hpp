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
#include "GameState.hpp"
#include "ActivePath.hpp"
#include <unordered_map>

class MoveController : public BaseController {

public:

	std::unordered_map <std::shared_ptr<GameObject>, std::shared_ptr<ActivePath>> _activePaths;

    MoveController();
    
    ~MoveController(){};
    
	virtual void attach(Observer* obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);

	virtual void updateActivePaths(float timestep, std::shared_ptr<GameState> state);

	virtual bool init(std::shared_ptr<GameState> state);

	static std::shared_ptr<MoveController> alloc(std::shared_ptr<GameState> state) {
		std::shared_ptr<MoveController> result = std::make_shared<MoveController>();
		return (result->init(state) ? result : nullptr);
	}

	/*
	* Returns a velocity vector which will move an object from start to end at "velocity"
	* pixels per frame.
	*/
	static cugl::Vec2 getVelocityVector(cugl::Vec2 start, cugl::Vec2 end, float velocity);

	// maybe in the future we can put these 2 functions in base controller; right now we can't
	// cuz level controller has different signature
	std::string serialize();
	std::shared_ptr<cugl::JsonValue> toJsonValue();

	void initAfterResume(std::shared_ptr<GameState> state, std::shared_ptr<cugl::JsonValue> rJson);
};

#endif /* MoveController_hpp */
