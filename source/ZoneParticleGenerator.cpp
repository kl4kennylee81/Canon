//
//  ZoneParticleGenerator.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ZoneParticleGenerator.hpp"

bool ZoneParticleGenerator::init(std::shared_ptr<ParticleNode> circlepartnode,
                                 std::shared_ptr<ParticleNode> ringpartnode,
                                 std::shared_ptr<cugl::FreeList<Particle>> mem,
                                 ParticleData circlepd,
                                 ParticleData ringpd) {
    _circlepartnode = circlepartnode;
    _ringpartnode = ringpartnode;
    _memory = mem;
    _active = false;
    
    // circle of the zone
    ParticleData circlepd_temp;
    circlepd_temp.color_fade = false;
    circlepd_temp.start_color = Color4f::RED;
    circlepd_temp.end_color = Color4f::RED;
    circlepd_temp.color_duration = -1; // infinite
    circlepd_temp.scale = true;
    circlepd_temp.current_scale = 0.65;
    circlepd_temp.start_scale = .65;
    circlepd_temp.end_scale = .65;
    
    
    // ring around the zone
    ParticleData ringpd_temp;
    ringpd_temp.color_fade = true;
    ringpd_temp.start_color = Color4f::RED;
    ringpd_temp.end_color = Color4f::RED;
    ringpd_temp.color_duration = 1; // infinite
    ringpd_temp.scale = true;
    ringpd_temp.current_scale = .5;
    ringpd_temp.start_scale = .5;
    ringpd_temp.end_scale = .5;
    ringpd_temp.rotate = true;
    
    
    _circlepd = circlepd_temp;
    _ringpd = ringpd_temp;
    
    _ring_num = 5;
    
    add_particles(Vec2(500,200));
    
    return true;
}

ParticleData ZoneParticleGenerator::randomizePD(ParticleData pd) {
    pd.revolution_time = ParticleGenerator::getRandomFloat(30,70);
    pd.current_angle = ParticleGenerator::getRandomFloat(0,M_PI_4);
    return pd;
}

void ZoneParticleGenerator::createZoneParticle(int num, std::set<Particle*>& particle_set) {
    Particle* circle = _memory->malloc();
    if (circle != nullptr) {
        particle_set.insert(circle);
        circle->init(_circlepd);
        _circlepartnode->addParticle(circle);
    }
    
    for (int ii = 0; ii < num; ++ii) {
        Particle* ring = _memory->malloc();
        if (ring != nullptr) {
            particle_set.insert(ring);
            ring->init(randomizePD(_ringpd));
            _ringpartnode->addParticle(ring);
        }
    }
}

void ZoneParticleGenerator::add_particles(cugl::Vec2 location) {
    Vec2* location_ptr = new Vec2(location); // temporary
    
    // create the wrapper
    std::set<Particle*> zone_particles_set;
    createZoneParticle(_ring_num, zone_particles_set);
    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(zone_particles_set, location);
    
    // insert wrapper to location -> wrapper map
    _location_to_wrapper.insert(std::make_pair(location_ptr, wrapper));
}

void ZoneParticleGenerator::updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset) {
    for (auto it=wrapper->_particle_set.begin(); it !=wrapper->_particle_set.end(); ++it) {
        Particle* p = *it;
        p->_pd.position = wrapper->_global_position;
        p->move();
        
        if (!p->isActive()) {
            reset.insert(p);
        }
    }
}

/**
 * Goes through all of the ParticleWrapper we have, and calls update on each one of them
 */
void ZoneParticleGenerator::generate() {
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



