//
//  PathController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathController_hpp
#define PathController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Observer.hpp"
#include "Event.hpp"
#include "GameState.hpp"

class PathController : public BaseController {
public:
    PathController();
    
	virtual void attach(std::shared_ptr<Observer> obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);

	virtual bool init();

	static std::shared_ptr<PathController> alloc() {
		std::shared_ptr<PathController> result = std::make_shared<PathController>();
		return (result->init() ? result : nullptr);
    }
};

#endif /* PathController_hpp */
