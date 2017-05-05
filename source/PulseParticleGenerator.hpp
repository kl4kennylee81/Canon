//
//  PulseParticleGenerator.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PulseParticleGenerator_hpp
#define PulseParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"

/**
 * Create a pulase in the given locations. Used to indicate spawning enemies.
 */
class PulseParticleGenerator : public ParticleGenerator {
private:
    /**
     * How many pulses to generate per second
     */
    float _pulse_rate;
    
    /**
     * Frames to wait until the next pulse is fired
     */
    float _timeout;
    
    /**
     * This will be replaced by GameObject -> ParticleWrapper
     */
    std::unordered_map<cugl::Vec2*, std::shared_ptr<ParticleWrapper>> _location_to_wrapper;
    
    void createPulseParticles(std::set<Particle*>& particle_set);
    
    ParticleData randomizeAngle(ParticleData pd);
    
    void updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset);
    
public:
    PulseParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<ParticleNode> partnode,
              std::shared_ptr<cugl::FreeList<Particle>> mem,
              ParticleData pd);
    
    static std::shared_ptr<PulseParticleGenerator> alloc(std::shared_ptr<ParticleNode> partnode, std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd) {
        std::shared_ptr<PulseParticleGenerator> result = std::make_shared<PulseParticleGenerator>();
        return (result->init(partnode, mem, pd) ? result : nullptr);
    }
    
    void generate();
    
    /**
     * Creates a fresh pulse particle from the template _pd and attaches it to the ParticleNode
     */
    Particle* createSinglePulse(ParticleData pd);
    
    /**
     * This is going to get replaced by the code that handles associating the zone with the particle wrapper
     * for now, associate a location with a particle wrapper
     */
    void add_particles(Vec2 location);
};


#endif /* PulseParticleGenerator_hpp */
