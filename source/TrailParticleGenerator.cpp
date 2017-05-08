//
//  TrailParticleGenerator.cpp
//  Canon
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TrailParticleGenerator.hpp"

#define PARTICLE_DELAY 0

bool TrailParticleGenerator::init(std::shared_ptr<cugl::FreeList<Particle>> mem, std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _memory = mem;
    _cooldown = PARTICLE_DELAY;
    _particle_map = particle_map;
    _active = false;
    
    // initialize separate ParticleNodes for blue and gold
    _bluepd = _particle_map->at("blue_particle");
    _bluepartnode = ParticleNode::allocWithTexture(_bluepd.texture);
    _bluepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _bluepartnode->setBlendEquation(GL_FUNC_ADD);
    _bluepartnode->setPosition(Vec2::ZERO);
    state->getWorldNode()->addChild(_bluepartnode);
    
    _goldpd = _particle_map->at("gold_particle");
    _goldpartnode = ParticleNode::allocWithTexture(_goldpd.texture);
    _goldpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _goldpartnode->setBlendEquation(GL_FUNC_ADD);
    _goldpartnode->setPosition(Vec2::ZERO);
    state->getWorldNode()->addChild(_goldpartnode);
    
    return true;
}

void TrailParticleGenerator::add_character(GameObject* char_obj) {
    std::cout<<"adding character\n";
    _character_list.push_back(char_obj);
}

void TrailParticleGenerator::generate_trail(GameObject* char_obj) {
    ElementType element = char_obj->getPhysicsComponent()->getElementType();
    Vec2 vel = char_obj->getPhysicsComponent()->getBody()->getLinearVelocity();
    
    if (vel.isZero()) return;
    
    if (_cooldown == 0) {
        Particle* sprite = _memory->malloc();
        if (sprite != nullptr) {
            float rand = getRandomFloat(0,1.0);
            auto angle = rand*2.0f*M_PI;
            
            ParticleData pd;
            std::shared_ptr<ParticleNode> partnode;
            if (element == ElementType::BLUE) {
                pd = _bluepd;
                partnode = _bluepartnode;
            }
            else if (element == ElementType::GOLD) {
                pd = _goldpd;
                partnode = _goldpartnode;
            }
            
            pd.position = char_obj->getPosition() * Util::getGamePhysicsScale();
            pd.velocity = Vec2(((float)(PARTICLE_SPEED*cosf(angle))),(float)(PARTICLE_SPEED*sinf(angle)));
            
            sprite->init(pd);
            partnode->addParticle(sprite);
            
            _cooldown = PARTICLE_DELAY;
        }
    } else if (_cooldown > 0) {
        _cooldown--;
    }
}

void TrailParticleGenerator::generate() {
    if (!_active) return;
    
    // Generate a trail for each character
    for (auto it = _character_list.begin(); it != _character_list.end(); it++) {
        GameObject* char_obj = (*it);
        generate_trail(char_obj);
    }
    
    // Move all of the particles according to velocity
    _bluepartnode->update(_particles);
    _goldpartnode->update(_particles);
    
    // Garbage collect particles
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Particle* p = *it;
        _bluepartnode->removeParticle(p);
        _goldpartnode->removeParticle(p);
        _memory->free(p);
    }
    
    _particles.clear();
}


