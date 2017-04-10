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
#include "BaseController.hpp"

class ParticleController : public BaseController {
public:
    ParticleController();
    
    virtual void attach(std::shared_ptr<Observer> obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);
    
    virtual bool init();
    
    static std::shared_ptr<ParticleController> alloc() {
        std::shared_ptr<ParticleController> result = std::make_shared<ParticleController>();
        return (result->init() ? result : nullptr);
    }
};


#endif /* ParticleController_hpp */


