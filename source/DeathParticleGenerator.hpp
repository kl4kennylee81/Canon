//
//  DeathParticleGenerator.hpp
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
    std::shared_ptr<ParticleNode> _bluepartnode; // one particlenode per texture for now
    std::shared_ptr<ParticleNode> _goldpartnode;
    
    ParticleData _blue_death_pd;
    ParticleData _gold_death_pd;
    
    std::set<std::shared_ptr<ParticleWrapper>> _alive_wrappers;
    
    void createDeathParticles(std::set<Particle*>& particle_set, ElementType element, Vec2 location);
    
    ParticleData randomizePD(ParticleData pd);
    
    void updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset);
    
public:
    DeathParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map);
    
    static std::shared_ptr<DeathParticleGenerator> alloc(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
        std::shared_ptr<DeathParticleGenerator> result = std::make_shared<DeathParticleGenerator>();
        return (result->init(state, particle_map) ? result : nullptr);
    }
    
    void generate();
    
    void add_particles(Vec2 location, ElementType element);
};


#endif /* DeathParticleGenerator_hpp */
