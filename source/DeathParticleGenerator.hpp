//
//  DeathParticleGenerator.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef DeathParticleGenerator_hpp
#define DeathParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"

/**
 *
 */
class DeathParticleGenerator : public ParticleGenerator {
private:
    std::set<std::shared_ptr<ParticleWrapper>> _alive_wrappers;
    
    void createDeathParticles(std::set<Particle*>& particle_set);
    ParticleData randomizePD(ParticleData pd);
    
    void updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset);
    
public:
    DeathParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<ParticleNode> partnode,
              std::shared_ptr<cugl::FreeList<Particle>> mem,
              ParticleData pd);
    
    static std::shared_ptr<DeathParticleGenerator> alloc(std::shared_ptr<ParticleNode> partnode, std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd) {
        std::shared_ptr<DeathParticleGenerator> result = std::make_shared<DeathParticleGenerator>();
        return (result->init(partnode, mem, pd) ? result : nullptr);
    }
    
    void generate();
    
    void add_particles(Vec2 location);
};


#endif /* DeathParticleGenerator_hpp */
