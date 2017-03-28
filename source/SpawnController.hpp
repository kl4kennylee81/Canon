//
//  SpawnController.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SpawnController_hpp
#define SpawnController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "BaseController.hpp"
#include "GameObject.hpp"

/**
 * The purpose of this controller is to delay spawning for 
 * SPAWN_FRAMES number of frames.
 */
class SpawnController : public BaseController {
protected:
    
    /**
     * Object that is about to spawn, and the frames that it waited so far
     */
    std::map<std::shared_ptr<GameObject>, float> spawnMap;
    
public:
    SpawnController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);
    
    virtual bool init();
    
    static std::shared_ptr<SpawnController> alloc() {
        std::shared_ptr<SpawnController> result = std::make_shared<SpawnController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* SpawnController_hpp */
