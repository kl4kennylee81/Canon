//
//  PulseParticleGenerator.cpp
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PulseParticleGenerator.hpp"
#include <map>
#include <math.h>

#define TIMEOUT_FRAMES 100

bool PulseParticleGenerator::init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _particle_map = particle_map;
    _active = false;
    
    // initialize partnode
    _ringpd = _particle_map->at("pulse_ring");
    _pulsepartnode = ParticleNode::allocWithTexture(_ringpd.texture);
    _pulsepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _pulsepartnode->setBlendEquation(GL_FUNC_ADD);
    _pulsepartnode->setPosition(Vec2::ZERO);
    _pulsepartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    state->getWorldNode()->addChild(_pulsepartnode);
    
    // initialize instance variables.
    _pulse_rate = 7; // pulses per ParticleWrapper
    _timeout = TIMEOUT_FRAMES;
    
    return true;
}

void PulseParticleGenerator::createPulseParticles(std::set<Particle*>& particle_set) {
    ParticleData pd = _ringpd; // don't taint the template
    
    // how much scale each pulse is separated by
    float scale_rate = ((float)(pd.end_scale - pd.start_scale)/(_pulse_rate-1));
    // how much ttl each pulse is separated by
    float ttl_rate = ceil(((float)pd.ttl)/(_pulse_rate-1));
    pd.current_scale = pd.start_scale;
    
    // create the particles taht are spaced out by a constant amount
    for (int ii = 0; ii < _pulse_rate; ii++) {
        Particle* particle = createSinglePulse(pd);
        particle_set.insert(particle);
        pd.current_scale += scale_rate; // ones in the outer ring are bigger
        pd.ttl -= ttl_rate; // ones in the outer ring die sooner
    }
}

Particle* PulseParticleGenerator::createSinglePulse(ParticleData pd) {
//    Particle* particle = _memory->malloc();
//    if (particle != nullptr) {
//        particle->init(randomizeAngle(pd)); // init makes a copy of pd
//        _pulsepartnode->addParticle(particle);
//    }
//    return particle;
    return nullptr;
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
//        p->_pd.position += Vec2(20,40);
        p->move();
        
        if (!p->isActive()) {
            to_remove.insert(p);
            
            if (wrapper->_repeat) {
                // make a brand new particle if we repeat
                Particle* particle = createSinglePulse(_ringpd);
                wrapper->_particle_set.insert(particle);
            }
        }
    }
    
    // remove from particle_list in the wrapper
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
//void PulseParticleGenerator::add_particles(cugl::Vec2 location) {
//    Vec2* location_ptr = new Vec2(location); // temporary
//    
//    // create the wrapper
//    std::set<Particle*> pulse_particles_set;
//    createPulseParticles(pulse_particles_set);
//    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(pulse_particles_set, location);
//    wrapper->setRepeat(true);
//    
//    // insert wrapper to location -> wrapper map
//    _location_to_wrapper.insert(std::make_pair(location_ptr, wrapper));
//}

void PulseParticleGenerator::add_mapping(GameObject* obj) {
    // don't add pulses on players
    if (obj->getIsPlayer()) return;
    
    // this might be the source of the gravity bug?
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    
    // create the wrapper
    std::set<Particle*> pulse_particles_set;
    createPulseParticles(pulse_particles_set);
    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(pulse_particles_set, world_pos);
    wrapper->setRepeat(true);
    
    // insert wrapper to object -> wrapper map
    _obj_to_wrapper.insert(std::make_pair(obj, wrapper));
}

void PulseParticleGenerator::remove_mapping(GameObject* obj) {
    
    if (obj->getIsPlayer()) return;
    
    auto wrapper = _obj_to_wrapper.at(obj);
    for(auto it = wrapper->_particle_set.begin(); it != wrapper->_particle_set.end(); ++it) {
        Particle* p = *it;
        _pulsepartnode->removeParticle(p);
//        _memory->free(p);
    }
    
    _obj_to_wrapper.erase(obj);
}

/**
 * Goes through all of the ParticleWrapper we have, and calls update on each one of them
 */
void PulseParticleGenerator::generate() {
    
    if (!_active) return;
    
    for (auto it = _obj_to_wrapper.begin(); it != _obj_to_wrapper.end(); it++) {
        GameObject* obj = it->first;
        std::shared_ptr<ParticleWrapper> wrapper = it->second;
        
        // sync the position of the wrapper to the character
        Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
        wrapper->_global_position = world_pos;
        
        updateWrapper(wrapper, _particles);
    }
    
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _pulsepartnode->removeParticle(p);
//        _memory->free(p);
    }
    _particles.clear();
}






