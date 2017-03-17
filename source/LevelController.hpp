//
//  LevelController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelController_hpp
#define LevelController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"
#include "World.hpp"
#include "Level.hpp"
#include "ProgressBarController.hpp"

class LevelController : BaseController {
protected:
    std::shared_ptr<World> _world;
    
    Level _level;
    
    std::shared_ptr<ProgressBarController> _progressBarController;
public:
    LevelController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<World> world);
    
    static std::shared_ptr<LevelController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
        std::shared_ptr<LevelController> result = std::make_shared<LevelController>();
        return (result->init(state, world) ? result : nullptr);
    }

	std::shared_ptr<World> getWorld() {
		return _world;
	}
};

#endif /* LevelController_hpp */
