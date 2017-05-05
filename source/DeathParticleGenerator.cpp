//
//  DeathParticleGenerator.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "DeathParticleGenerator.hpp"

#define PARTICLE_BASE_SPEED 5
#define CLUSTER_SIZE 5

bool DeathParticleGenerator::init(std::shared_ptr<cugl::FreeList<Particle>> mem,
                                  ParticleData pd, std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _memory = mem;
    _pd = pd;
    _active = false; // default
    _particle_map = particle_map;
    
    // initialize particle node and attach to the world node
    ParticleData blue_death_pd = _particle_map->at("blue_death");
    _bluepartnode = ParticleNode::allocWithTexture(blue_death_pd.texture);
    _bluepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _bluepartnode->setPosition(Vec2::ZERO);
    _bluepartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    state->getWorldNode()->addChild(_bluepartnode);
    
    ParticleData gold_death_pd = _particle_map->at("green_death");
    _goldpartnode = ParticleNode::allocWithTexture(gold_death_pd.texture);
    _goldpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _goldpartnode->setPosition(Vec2::ZERO);
    _goldpartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    state->getWorldNode()->addChild(_goldpartnode);
    
    return true;
}

ParticleData DeathParticleGenerator::randomizePD(ParticleData pd) {
    float rand = getRandomFloat(0, 1);
    float ttl_rand = getRandomFloat(10, 50);
    
    auto angle = rand*2.0f*M_PI;
    pd.velocity = Vec2((PARTICLE_BASE_SPEED*(float)(cosf(angle))),PARTICLE_BASE_SPEED*(float)(sinf(angle)));
    
    pd.ttl = floor(ttl_rand);
    pd.revolution_time = rand*5 + 2;
    return pd;
}

void DeathParticleGenerator::createDeathParticles(std::set<Particle*>& particle_set, ElementType element) {
    for (int ii = 0; ii < CLUSTER_SIZE; ++ii) {
        Particle* particle = _memory->malloc();
        if (particle != nullptr) {
            particle->init(randomizePD(_pd));
            particle_set.insert(particle);
            if (element == ElementType::BLUE) {
                _bluepartnode->addParticle(particle);
            } else {
                _goldpartnode->addParticle(particle);
            }
        }
    }
}

void DeathParticleGenerator::add_particles(Vec2 location, ElementType element) {
    if (!_active) return;
    
    // just don't use the global position for this one
    _pd.position = location;
    
    // create the wrapper
    std::set<Particle*> death_particles_set;
    createDeathParticles(death_particles_set, element);
    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(death_particles_set, location);
    
    _alive_wrappers.insert(wrapper);
}

void DeathParticleGenerator::updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset) {
    std::set<Particle*> to_remove;
    
    for (auto it=wrapper->_particle_set.begin(); it !=wrapper->_particle_set.end(); ++it) {
        Particle* p = *it;
        p->move();
        
        if (!p->isActive()) {
            reset.insert(p);
            to_remove.insert(p);
        }
    }
    
    // remove from particle_list
    for (auto it=to_remove.begin(); it != to_remove.end(); it++) {
        wrapper->_particle_set.erase(*it);
    }
}

void DeathParticleGenerator::generate() {
    if (!_active) return;
    
    std::set<std::shared_ptr<ParticleWrapper>> to_remove;
    
    for (auto it = _alive_wrappers.begin(); it != _alive_wrappers.end(); it++) {
        std::shared_ptr<ParticleWrapper> wrapper = (*it);
        if (wrapper->_particle_set.size() == 0) {
            to_remove.insert(wrapper);
            break;
        }
        updateWrapper(wrapper, _particles);
    }
    
    // remove from alive wrappers set
    for (auto it=to_remove.begin(); it != to_remove.end(); it++) {
        _alive_wrappers.erase(*it);
    }
    
    // remove the dead particles from the particle node
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _bluepartnode->removeParticle(p); // check both since we don't know
        _goldpartnode->removeParticle(p);
        _memory->free(p);
    }
    _particles.clear();
}
