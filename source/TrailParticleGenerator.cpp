//
//  TrailParticleGenerator.cpp
//  Canon
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TrailParticleGenerator.hpp"

void TrailParticleGenerator::generate() {
    
    auto vel = _char->getPhysicsComponent()->getBody()->getLinearVelocity();
    if (vel.isZero()) {
        _active = false;
    } else {
        _active = true;
    }
    
    if (_active && _cooldown == 0) {
        Particle* sprite = _memory->malloc();
        
        if (sprite != nullptr) {
            // Set random trajectory
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> dist(0, 1.0f);
            float rand = dist(mt);
            auto angle = rand*2.0f*M_PI;
            
            _pd.position = _char->getPosition() * Util::getGamePhysicsScale();
            _pd.velocity = Vec2( ((float)(PARTICLE_SPEED*cosf(angle))),(float)(PARTICLE_SPEED*sinf(angle)) );
            
            sprite->init(_pd);
            _partnode->addParticle(sprite);
            
            _cooldown = _respawn;
        }
        
    } else if (_cooldown > 0) {
        _cooldown--;
    }
    
    // Move all of the particles according to velocity
    _partnode->update(_particles);
    
    // Garbage collect particles that go out of bounds
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _partnode->removeParticle(p);
        _memory->free(p);
    }
    
    _particles.clear();
}


