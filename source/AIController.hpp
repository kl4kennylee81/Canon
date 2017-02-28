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

class AIController : public BaseController {

public:
    
    AIController();

	virtual void attach(std::shared_ptr<Observer> obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);

	virtual bool init();

	static std::shared_ptr<AIController> alloc() {
		std::shared_ptr<AIController> result = std::make_shared<AIController>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* AIController_hpp */
