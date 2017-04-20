//
//  ParticleGenerator.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleGenerator_hpp
#define ParticleGenerator_hpp

#include <stdio.h>
#include "Particle.h"
#include "ParticleData.hpp"

/**
 * Base particle generator
 */
class ParticleGenerator {
protected:
    std::shared_ptr<ParticleNode> _partnode;
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    ParticleData _pd;
    /** Flag to start or stop the generator */
    bool _active;
    
public:
    ParticleGenerator() {}
    
    ParticleGenerator(std::shared_ptr<ParticleNode> partnode, std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd) {
        _partnode = partnode;
        _memory = mem;
        _pd = pd;
    }
    
    /** Start / Stop generator */
    virtual void start() { _active = true; };
    virtual void stop() { _active = false; };
    
    /** Generate one step of the particles */
    virtual void generate() {};
};


/**
 * Particle generator that makes a trail of particles behind the given character
 * TODO Commented out the code regarding GameObject because we are testing currently in a demo
 */
class TrailParticleGenerator : public ParticleGenerator {
private:
    //    std::shared_ptr<GameObject> _char;
    
public:
    TrailParticleGenerator() : ParticleGenerator() {}
    
    TrailParticleGenerator(std::shared_ptr<ParticleNode> partnode,
                           std::shared_ptr<cugl::FreeList<Particle>> mem,
                           ParticleData pd
    /* std::shared_ptr<GameObject> char */ ) : ParticleGenerator(partnode, mem, pd) {
        //        _char = char;
    }
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<TrailParticleGenerator> alloc() {
        std::shared_ptr<TrailParticleGenerator> result = std::make_shared<TrailParticleGenerator>();
        return (result->init() ? result : nullptr);
    }
    
    /** TEMP: Generates particles of type _pd where the mouse was clicked */
    void generate() {
        if (!_active) return;
        std::cout << "particle generated\n";
    }
};

/**
 * Randomly generates
 */
class BackgroundParticleGenerator : public ParticleGenerator {
public:
    BackgroundParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<ParticleNode> partnode,
              std::shared_ptr<cugl::FreeList<Particle>> mem,
              ParticleData pd) {
        _partnode = partnode;
        _memory = mem;
        _pd = pd;
        return true;
    }
    
    void generate() {
        if (!_active) return;
        
        // generate random particles on the map
    }
};

#endif /* ParticleGenerator_hpp */
