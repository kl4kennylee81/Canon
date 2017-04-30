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
#include "GameObject.hpp"
#include "GameState.hpp"
#include <random>


/**
 * Base particle generator
 */
class ParticleGenerator {
protected:
    /** The node that renders all particles for this generator */
    std::shared_ptr<ParticleNode> _partnode;
    
    std::shared_ptr<cugl::FreeList<Particle>> _memory;
    ParticleData _pd;
    /** Flag to start or stop the generator */
    bool _active;
    
public:
    ParticleGenerator() {}
    
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
    // need to keep track of the character to get positions
    std::shared_ptr<GameObject> _char;
    int _cooldown;
    int _respawn;
    std::set<Particle*> _particles;
    
public:
    TrailParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> ch, std::shared_ptr<GameState> state) {
        _memory = mem;
        _pd = pd;
        _char = ch;
        _respawn = 0;
        _cooldown = _respawn;
        
        // initialize particle node and attach to the world node
        _partnode = ParticleNode::allocWithTexture(_pd.texture);
        _partnode->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        _partnode->setPosition(Vec2::ZERO);
        _partnode->setContentSize(state->getWorldNode()->getContentSize());
        state->getWorldNode()->addChild(_partnode);
    
        return true;
    }
    
    static std::shared_ptr<TrailParticleGenerator> alloc(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> gameObj, std::shared_ptr<GameState> state) {
        std::shared_ptr<TrailParticleGenerator> result = std::make_shared<TrailParticleGenerator>();
        return (result->init(mem, pd, gameObj, state) ? result : nullptr);
    }
    
    void generate();
};


/**
 * Randomly generates some particles on a map to make it look alive
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
