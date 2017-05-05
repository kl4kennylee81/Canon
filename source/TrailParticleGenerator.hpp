//
//  TrailParticleGenerator.hpp
//  Canon
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TrailParticleGenerator_hpp
#define TrailParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"
#include "Util.hpp"

/**
 * Particle generator that makes a trail of particles behind the given character
 * TODO Commented out the code regarding GameObject because we are testing currently in a demo
 */
class TrailParticleGenerator : public ParticleGenerator {
private:
    // need to keep track of the character to get positions
    std::shared_ptr<GameObject> _char;
    int _cooldown;
    int _respawn;
    std::set<Particle*> _particles;
    
public:
    TrailParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> ch, std::shared_ptr<GameState> state);
    
    static std::shared_ptr<TrailParticleGenerator> alloc(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> gameObj, std::shared_ptr<GameState> state) {
        std::shared_ptr<TrailParticleGenerator> result = std::make_shared<TrailParticleGenerator>();
        return (result->init(mem, pd, gameObj, state) ? result : nullptr);
    }
    
    void generate();
};


#endif /* TrailParticleGenerator_hpp */
