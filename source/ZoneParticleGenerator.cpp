//
//  ZoneParticleGenerator.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ZoneParticleGenerator.hpp"

bool ZoneParticleGenerator::init(std::shared_ptr<cugl::FreeList<Particle>> mem, std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _particle_map = particle_map;
    _memory = mem;
    _active = false;
    
    // initialize particle node and attach to the world node
    ParticleData circlepd = _particle_map->at("t_portalbackground");
    _circlepartnode = ParticleNode::allocWithTexture(circlepd.texture);
    _circlepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _circlepartnode->setBlendEquation(GL_FUNC_ADD);
    _circlepartnode->setPosition(Vec2::ZERO);
    _circlepartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    state->getWorldNode()->addChild(_circlepartnode);
    
    // initialize particle node and attach to the world node
    ParticleData ringpd = _particle_map->at("t_portalcircle");
    _ringpartnode = ParticleNode::allocWithTexture(ringpd.texture);
    _ringpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _ringpartnode->setBlendEquation(GL_FUNC_ADD);
    _ringpartnode->setPosition(Vec2::ZERO);
    _ringpartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    state->getWorldNode()->addChild(_ringpartnode);
    
    _circlepd = circlepd;
    _ringpd = ringpd;
    _ring_num = 5;
    
    return true;
}

ParticleData ZoneParticleGenerator::randomizePD(ParticleData pd) {
    pd.revolution_time = ParticleGenerator::getRandomFloat(30,70);
    pd.current_angle = ParticleGenerator::getRandomFloat(0,M_PI_4);
    return pd;
}

void ZoneParticleGenerator::createZoneParticle(int num, std::set<Particle*>& particle_set, ElementType element) {
    if (element == ElementType::BLUE) {
        _circlepd.start_color = Color4f::BLUE;
        _circlepd.end_color = Color4f::BLUE;
        _ringpd.start_color = Color4f::BLUE;
        _ringpd.end_color = Color4f::BLUE;
    } else {
        _circlepd.start_color = Color4f::YELLOW;
        _circlepd.end_color = Color4f::YELLOW;
        _ringpd.start_color = Color4f::YELLOW;
        _ringpd.end_color = Color4f::YELLOW;
    }
    
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

void ZoneParticleGenerator::add_mapping(GameObject* obj) {
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    std::set<Particle*> zone_particles_set;
    createZoneParticle(_ring_num, zone_particles_set, obj->getPhysicsComponent()->getElementType());
    std::shared_ptr<ParticleWrapper> wrapper = ParticleWrapper::alloc(zone_particles_set, world_pos);
    
    _obj_to_wrapper.insert(std::make_pair(obj, wrapper));
}

void ZoneParticleGenerator::remove_mapping(GameObject* obj) {
    auto wrapper = _obj_to_wrapper.at(obj);
    for(auto it = wrapper->_particle_set.begin(); it != wrapper->_particle_set.end(); ++it) {
        Particle* p = *it;
        _circlepartnode->removeParticle(p);
        _ringpartnode->removeParticle(p);
        _memory->free(p);
    }
    
    _obj_to_wrapper.erase(obj);
    
}

void ZoneParticleGenerator::updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset) {
    // update _global_position by looking at the zone position;
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
    
    for (auto it = _obj_to_wrapper.begin(); it != _obj_to_wrapper.end(); it++) {
        
        GameObject* obj = it->first;
        std::shared_ptr<ParticleWrapper> wrapper = it->second;
        
        // sync zone position with char position.
        Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
        wrapper->_global_position = world_pos;
        
        // update particles
        updateWrapper(wrapper, _particles);
    }
    
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _circlepartnode->removeParticle(p);
        _ringpartnode->removeParticle(p);
        _memory->free(p);
    }
    _particles.clear();
}



