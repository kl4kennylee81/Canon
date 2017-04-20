//
//  ParticleController.hpp
//  Canon
//
//  Created by Hong Jeon on 4/10/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleController_hpp
#define ParticleController_hpp

#include <stdio.h>
#include <set>
#include "BaseController.hpp"
#include "Particle.h"

class ParticleController : public BaseController {
private:
    /** The memory pool for future allocations */
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    
public:
    ParticleController();
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep, std::shared_ptr<GameState> state);
    
    virtual bool init();
    
    static std::shared_ptr<ParticleController> alloc() {
        std::shared_ptr<ParticleController> result = std::make_shared<ParticleController>();
        return (result->init() ? result : nullptr);
    }
};


#endif /* ParticleController_hpp */


