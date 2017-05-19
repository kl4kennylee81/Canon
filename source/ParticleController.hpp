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
#include "DeathParticleGenerator.hpp"
#include "ZoneParticleGenerator.hpp"
#include "PulseParticleGenerator.hpp"
#include "PathParticleGenerator.hpp"
#include "Util.hpp"
#include "ActiveParticleState.hpp"
#include "ParticleStateData.hpp"

class ParticleController : public BaseController {
private:
    /** Can retreive specific particles by name */
    std::unordered_map<std::string, ParticleData> _particle_map;
    
    /** Generators */
    std::shared_ptr<DeathParticleGenerator> _death_gen;
    std::shared_ptr<ZoneParticleGenerator> _zone_gen;
    std::shared_ptr<TrailParticleGenerator> _trail_gen;
    std::shared_ptr<PulseParticleGenerator> _pulse_gen;
    std::shared_ptr<PathParticleGenerator> _path_gen;
    
    /** Handlers for events */
    void handleCharacterSpawn(GameObject* obj);
    void handleCharacterDeath(GameObject* obj);
    void handleDeathParticle(GameObject* obj);
    void handleZoneSpawn(GameObject* obj);
    
    std::map<GameObject*, std::shared_ptr<ActiveParticleState>> objectStateMap;
    
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
    
    void addObject(GameObject* obj, std::shared_ptr<ParticleStateData>);
    
    void handleAction(GameObject* obj, AnimationAction animAction);
};


#endif /* ParticleController_hpp */


