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
#include "PhysicsComponent.hpp"
#include "Util.hpp"

using namespace cugl;

/**
 * Base particle generator
 */
class ParticleGenerator {
protected:
    /** The set of particles to be removed at the end of update()*/
    std::set<Particle*> _particles;
    
    /**
     * The map that keeps all of the particle data by name.
     */
    std::unordered_map<std::string, ParticleData>* _particle_map;
    
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
