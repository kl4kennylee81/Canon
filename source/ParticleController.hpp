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
#include "ParticleGenerator.hpp"
#include "TrailParticleGenerator.hpp"
#include "ParticleGenerator.hpp"

class ParticleController : public BaseController {
private:
    /** The memory pool for future allocations */
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    
    /** Character to the trail generator map */
    std::unordered_map<GameObject*, std::shared_ptr<TrailParticleGenerator>> _character_map;
    
    /** Can retreive specific particles by name */
    std::unordered_map<std::string, ParticleData> _particle_map;
    
    void generateTrails();
    
public:
    ParticleController();
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep, std::shared_ptr<GameState> state);
    
    virtual bool init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets);
    
    static std::shared_ptr<ParticleController> alloc(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
        std::shared_ptr<ParticleController> result = std::make_shared<ParticleController>();
        return (result->init(state, assets) ? result : nullptr);
    }
};


#endif /* ParticleController_hpp */


