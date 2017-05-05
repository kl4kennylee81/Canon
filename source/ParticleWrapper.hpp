//
//  ParticleWrapper.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleWrapper_hpp
#define ParticleWrapper_hpp

#include <stdio.h>
#include <set>
#include "Particle.h"

/**
 * Sometimes there is a need to move groups of particles as one entity. This class
 * encapsulates the methods and states that allow for transformation and updates
 * to happen as a group.
 */
class ParticleWrapper {
public:
    /**
     * Resetting the ttl of the particle after it has died. Mainly for repeating effects
     * like pulsing particle.
     */
    bool _repeat;
    
    /**
     * The position for all particles encapulated by this wrapper. In update, all the particles
     * positions get updates to match the global position.
     */
    Vec2 _global_position;
    
    /**
     * All the particles that this wrapper encapsulates. All updates are made as one.
     */
    std::set<Particle*> _particle_set;
    
    /**
     * This is mainly for the encapsulating the pulse particle.
     */
    void setRepeat(bool r) { _repeat = r; }
    
    bool init(std::set<Particle*>& particle_list, Vec2 position) {
        _particle_set = particle_list;
        _repeat = false; // default
        _global_position = position;
        return true;
    }
    
    static std::shared_ptr<ParticleWrapper> alloc(std::set<Particle*>& particle_set, Vec2 position) {
        std::shared_ptr<ParticleWrapper> result = std::make_shared<ParticleWrapper>();
        return (result->init(particle_set, position) ? result : nullptr);
    }
};

#endif /* ParticleWrapper_hpp */
