//
//  SwitchController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SwitchController_hpp
#define SwitchController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"
#include "Event.hpp"


class SwitchController : public BaseController {
public:
    bool switchFlag;
    bool spawnSwitch;

	std::unordered_map <std::shared_ptr<GameObject>, cugl::Color4> _baseColors;

	SwitchController();
    
    ~SwitchController(){};

	virtual void attach(std::shared_ptr<Observer> obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);

	virtual void update(float timestep, std::shared_ptr<GameState> state);

	virtual bool init(std::shared_ptr<GameState> state);

	static std::shared_ptr<SwitchController> alloc(std::shared_ptr<GameState> state) {
		std::shared_ptr<SwitchController> result = std::make_shared<SwitchController>();
		return (result->init(state) ? result : nullptr);
	}
};

#endif /* SwitchController_hpp */
