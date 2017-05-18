//
//  TrailParticleGenerator.cpp
//  Canon
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TrailParticleGenerator.hpp"

#define PARTICLE_DELAY 0
#define PARTICLE_COUNT 2
#define MAX_PARTICLES 2000

bool TrailParticleGenerator::init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _cooldown = PARTICLE_DELAY;
    _particle_map = particle_map;
    _active = false;
    
    // initialize separate ParticleNodes for blue and gold
    _bluepd = _particle_map->at("blue_trail");
    _bluepartnode = ParticleNode::allocWithTexture(_bluepd.texture);
    _bluepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _bluepartnode->setBlendEquation(GL_FUNC_ADD);
    _bluepartnode->setPosition(Vec2::ZERO);
    _bluepartnode->init_memory(MAX_PARTICLES);
    state->getWorldNode()->addChild(_bluepartnode);
    
    // gold particle node
    _goldpd = _particle_map->at("gold_trail");
    _goldpartnode = ParticleNode::allocWithTexture(_goldpd.texture);
    _goldpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _goldpartnode->setBlendEquation(GL_FUNC_ADD);
    _goldpartnode->setPosition(Vec2::ZERO);
    _goldpartnode->init_memory(MAX_PARTICLES);
    state->getWorldNode()->addChild(_goldpartnode);
    
    return true;
}

void TrailParticleGenerator::add_character(GameObject* char_obj) {
    _character_list.push_back(char_obj);
}

void TrailParticleGenerator::remove_character(GameObject* char_obj) {
    for(auto it = _character_list.begin() ; it != _character_list.end(); ++it) {
        if ((*it) == char_obj){
            it = _character_list.erase(it);
            break;
        }
    }
}

void TrailParticleGenerator::generate_trail(GameObject* char_obj) {
    ElementType element = char_obj->getPhysicsComponent()->getElementType();
    Vec2 vel = char_obj->getPhysicsComponent()->getBody()->getLinearVelocity();
    Vec2 world_pos = char_obj->getPosition() * Util::getGamePhysicsScale();
    bool active;
    
    // don't make the trail if the character is not moving
    if (vel.isZero()) {
        active = false;
    } else {
        active = true;
    }
    
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
    
    if (active && _cooldown == 0) {
        createTrailParticle(PARTICLE_COUNT, pd, partnode, world_pos);
        _cooldown = PARTICLE_DELAY;
    } else if (_cooldown > 0) {
        _cooldown--;
    }
}

void TrailParticleGenerator::createTrailParticle(int num, ParticleData pd, std::shared_ptr<ParticleNode> partnode, Vec2 world_pos) {
    for (int ii = 0; ii < num; ii++) {
        float rand = getRandomFloat(0,1.0);
        auto angle = rand*2.0f*M_PI;
        
        pd.position = world_pos;
        pd.velocity = Vec2(((float)(PARTICLE_SPEED*cosf(angle))),(float)(PARTICLE_SPEED*sinf(angle)));
        
        partnode->addParticle(pd, -1); // -1 means no group
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
    _bluepartnode->update();
    _goldpartnode->update();
}


