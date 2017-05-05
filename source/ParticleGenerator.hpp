//
//  ParticleGenerator.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleGenerator_hpp
#define ParticleGenerator_hpp

#include <cugl/cugl.h>
#include <stdio.h>
#include <random>
#include "Particle.h"
#include "ParticleData.hpp"
#include "ParticleNode.hpp"
#include "ParticleWrapper.hpp"
#include "GameObject.hpp"
#include "GameState.hpp"

using namespace cugl;

/**
 * Base particle generator
 */
class ParticleGenerator {
protected:
    /**
     * Each particle node is associated with one texture. If there is more than one
     * particle node controlled by a generator, then you can make separate variables for those.
     */
    std::shared_ptr<ParticleNode> _partnode;
    
    /** Initialized and held in ParticleController */
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    
    /** The set of particles to be removed at the end of update()*/
    std::set<Particle*> _particles;
    
    /** Copy of the particle data */
    ParticleData _pd;
    
    /** Flag to start or stop the generator */
    bool _active;
    
public:
    ParticleGenerator() {}
    
    /** Start/Stop generator */
    virtual void start() { _active = true; };
    virtual void stop() { _active = false; };
    
    /** Generate one step of the particles */
    virtual void generate() = 0;
    
    /** Uniform random generator [a,b) */
    static float getRandomFloat(float a, float b);
};

#endif /* ParticleGenerator_hpp */
