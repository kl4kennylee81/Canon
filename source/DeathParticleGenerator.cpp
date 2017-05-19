//
//  DeathParticleGenerator.cpp
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "DeathParticleGenerator.hpp"

#define PARTICLE_BASE_SPEED 5
#define CLUSTER_SIZE 2

// max number of pulse particles
#define MAX_PARTICLES 30
#define MAX_GROUPS 1

bool DeathParticleGenerator::init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _active = false; // default
    _particle_map = particle_map;
    _groups = GroupContainer::alloc(MAX_GROUPS);
    
    // initialize particle node and attach to the world node
    _blue_death_pd = _particle_map->at("blue_death");
    _bluepartnode = ParticleNode::allocWithTexture(_blue_death_pd.texture);
    _bluepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _bluepartnode->setBlendEquation(GL_FUNC_ADD);
    _bluepartnode->setPosition(Vec2::ZERO);
    _bluepartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    
    // important steps
    _bluepartnode->init_memory(MAX_PARTICLES);
    _bluepartnode->_groups = _groups;
    
    state->getWorldNode()->addChild(_bluepartnode);
    
    _gold_death_pd = _particle_map->at("gold_death");
    _goldpartnode = ParticleNode::allocWithTexture(_gold_death_pd.texture);
    _goldpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _goldpartnode->setBlendEquation(GL_FUNC_ADD);
    _goldpartnode->setPosition(Vec2::ZERO);
    _goldpartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    
    // important steps
    _goldpartnode->init_memory(MAX_PARTICLES);
    _goldpartnode->_groups = _groups;
    
    state->getWorldNode()->addChild(_goldpartnode);
    
    return true;
}

ParticleData DeathParticleGenerator::randomizePD(ParticleData pd) {
    float rand = getRandomFloat(0, 1);
    float ttl_rand = getRandomFloat(5, 30);
    
    auto angle = rand*2.0f*M_PI;
    pd.velocity = Vec2((PARTICLE_BASE_SPEED*(float)(cosf(angle))),PARTICLE_BASE_SPEED*(float)(sinf(angle)));
    
    pd.ttl = floor(ttl_rand);
    pd.revolution_time = rand*5 + 2;
    return pd;
}

void DeathParticleGenerator::add_particles(Vec2 location, ElementType element) {
    for (int ii = 0; ii < CLUSTER_SIZE; ++ii) {
        if (element == ElementType::BLUE) {
            ParticleData pd = _blue_death_pd; // copy template
            pd.position = location;
            _bluepartnode->addParticle(randomizePD(pd), -1, pd);
        } else {
            ParticleData pd = _gold_death_pd; // copy template
            pd.position = location;
            _goldpartnode->addParticle(randomizePD(pd), -1, pd);
        }
    }
}

void DeathParticleGenerator::generate() {
    if (!_active) return;
    
    _bluepartnode->update();
    _goldpartnode->update();
}
