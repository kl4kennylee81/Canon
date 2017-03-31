//
//  AIController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AIController_hpp
#define AIController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Event.hpp"
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "WaveData.hpp"
#include "AIData.hpp"

class AIController : public BaseController {

public:
	std::unordered_set<std::shared_ptr<ActiveAI>> _enemies;

	std::unordered_map<GameObject*, std::shared_ptr<ActiveAI>> _map;

    AIController();

	virtual void attach(Observer* obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);

	virtual bool init();

	void addAI(std::shared_ptr<ActiveAI> ai);

	void removeAI(GameObject* obj);

	static std::shared_ptr<AIController> alloc() {
		std::shared_ptr<AIController> result = std::make_shared<AIController>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* AIController_hpp */
