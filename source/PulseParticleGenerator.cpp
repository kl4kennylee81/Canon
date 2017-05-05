//
//  PulseParticleGenerator.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PulseParticleGenerator.hpp"
#include <map>
#include <math.h>

#define TIMEOUT_FRAMES 100

bool PulseParticleGenerator::init(std::shared_ptr<ParticleNode> partnode,
                                  std::shared_ptr<cugl::FreeList<Particle>> mem,
                                  ParticleData pd) {
    _partnode = partnode;
    _memory = mem;
    
    ParticleData pd_temp;
    pd_temp.ttl = 180;
    
    // fix this later
    pd_temp.color_fade = true;
    pd_temp.start_color = Color4f::WHITE;
    pd_temp.end_color = Color4f::WHITE;
    pd_temp.color_duration = -1; // infinite
    
    pd_temp.scale = true;
    pd_temp.start_scale = 0.05;
    pd_temp.end_scale = 0.4;
    pd_temp.current_scale = pd_temp.start_scale;
    
    pd_temp.alpha_fade = true;
    pd_temp.start_alpha = 0.5;
    pd_temp.alpha_duration = 120;
    
    _pd = pd_temp;
    _pulse_rate = 7; // pulses per ParticleWrapper
    _timeout = TIMEOUT_FRAMES;
    
    return true;
}

void PulseParticleGenerator::createPulseParticles(std::set<Particle*>& particle_set) {
    ParticleData pd = _pd; // don't taint the template
    
    // how much scale each pulse is separated by
    float scale_rate = ((float)(pd.end_scale - pd.start_scale)/(_pulse_rate-1));
    // how much ttl each pulse is separated by
    float ttl_rate = ceil(((float)pd.ttl)/(_pulse_rate-1));
    pd.current_scale = pd.start_scale;
    
    // create the particles taht are spaced out by a constant amount
    for (int ii = 0; ii < _pulse_rate; ii++) {
        Particle* particle = createSinglePulse(pd);
        particle_set.insert(particle);
        pd.current_scale += scale_rate;
        pd.ttl -= ttl_rate;
    }
}

Particle* PulseParticleGenerator::createSinglePulse(ParticleData pd) {
    Particle* particle = _memory->malloc();
    if (particle != nullptr) {
        particle->init(randomizeAngle(pd)); // init makes a copy of pd
        _partnode->addParticle(particle);
    }
    return particle;
}

ParticleData PulseParticleGenerator::randomizeAngle(ParticleData pd) {
    float rand = getRandomFloat(0,1);
    auto angle = rand*2.0f*M_PI;
    pd.current_angle = angle;
    return pd;
}

void PulseParticleGenerator::updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset) {
    std::set<Particle*> to_remove;
    
    //    if (wrapper->_particle_set.size() < _pulse_rate && _timeout >= TIMEOUT_FRAMES) {
    //        Particle* particle = createSinglePulse(_pd);
    //        wrapper->_particle_set.insert(particle);
    //        _timeout = 0;
    //    }
    
    for (auto it=wrapper->_particle_set.begin(); it !=wrapper->_particle_set.end(); ++it) {
        Particle* p = *it;
        
        // update all particles to be in the position of the wrapper
        p->_pd.position = wrapper->_global_position;
        p->move();
        
        if (!p->isActive()) {
            to_remove.insert(p);
            
            if (wrapper->_repeat) {
                // make a brand new particle
                Particle* particle = createSinglePulse(_pd);
                wrapper->_particle_set.insert(particle);
            }
        }
    }
    
    // remove from particle_list
    for (auto it=to_remove.begin(); it != to_remove.end(); it++) {
        wrapper->_particle_set.erase(*it);
    }
    
    // this removes it from the actual ParticleNode
    reset.insert(to_remove.begin(), to_remove.end());
    
    _timeout++;
}

/**
 * Makes a ParticleWrapper in the given location
 * and adds particles to the mapping Location -> ParticleWrapper
 */
void PulseParticleGenerator::add_particles(cugl::Vec2 location) {
    Vec2* location_ptr = new Vec2(location); // temporary
    
    // create the wrapper
    std::set<Particle*> pulse_particles_set;
    createPulseParticles(pulse_particles_set);
    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(pulse_particles_set, location);
    wrapper->setRepeat(true);
    
    // insert wrapper to location -> wrapper map
    _location_to_wrapper.insert(std::make_pair(location_ptr, wrapper));
}

/**
 * Goes through all of the ParticleWrapper we have, and calls update on each one of them
 */
void PulseParticleGenerator::generate() {
    if (!_active) return;
    
    for (auto it = _location_to_wrapper.begin(); it != _location_to_wrapper.end(); it++) {
        std::shared_ptr<ParticleWrapper> wrapper = it->second;
        updateWrapper(wrapper, _particles);
    }
    
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _partnode->removeParticle(p);
        _memory->free(p);
    }
    _particles.clear();
}






