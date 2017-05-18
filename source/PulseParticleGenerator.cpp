//
//  PulseParticleGenerator.cpp
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PulseParticleGenerator.hpp"
#include <map>
#include <math.h>

// how many frames to wait until the next pulse
#define TIMEOUT_FRAMES 100
// how many pulses to generate per second
#define PULSE_RATE 7
// max number of pulse particles
#define MAX_PARTICLES 1000
#define MAX_GROUPS 1000

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
    
    // important steps
    _groups = GroupContainer::alloc(MAX_GROUPS);
    _pulsepartnode->init_memory(MAX_PARTICLES);
    _pulsepartnode->_groups = _groups;
    
    state->getWorldNode()->addChild(_pulsepartnode);
    
    // initialize instance variables.
    _timeout = TIMEOUT_FRAMES;
    
    return true;
}

ParticleData PulseParticleGenerator::randomizeAngle(ParticleData pd) {
    float rand = getRandomFloat(0,1);
    auto angle = rand*2.0f*M_PI;
    pd.current_angle = angle;
    return pd;
}

/**
 * Generates a pulse at world_pos and assigns a new group to that pulse.
 */
void PulseParticleGenerator::createPulseParticles(Vec2 world_pos, int group_num) {
    ParticleData pd = _ringpd; // don't taint the template
    
    // how much scale each pulse is separated by
    float scale_rate = ((float)(pd.end_scale - pd.start_scale)/(PULSE_RATE-1));
    // how much ttl each pulse is separated by
    float ttl_rate = ceil(((float)pd.ttl)/(PULSE_RATE-1));
    pd.current_scale = pd.start_scale;
    
    // create the particles that are spaced out by a constant amount
    for (int ii = 0; ii < PULSE_RATE; ii++) {
        _pulsepartnode->addParticle(randomizeAngle(pd), group_num);
        pd.current_scale += scale_rate; // ones in the outer ring are bigger
        pd.ttl -= ttl_rate; // ones in the outer ring die sooner
    }
}

void PulseParticleGenerator::add_mapping(GameObject* obj) {
    // don't add pulses on players
    if (obj->getIsPlayer()) return;
    
    // this might be the source of the gravity bug?
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    bool repeat = true;
    
    // finds the next available group num
    int group_num = _groups->makeNewGroup(world_pos, repeat);
    
    // create the wrapper
    createPulseParticles(world_pos, group_num);
    
    // insert wrapper to object -> wrapper map
    _obj_to_group_num.insert(std::make_pair(obj, group_num));
    
    std::cout <<"added mapping\n";
    
}

void PulseParticleGenerator::remove_mapping(GameObject* obj) {
    if (obj->getIsPlayer()) return;
    
    // find out what group the object belongs to
    auto group_num = _obj_to_group_num.at(obj);
    
    // mark this one as done. very important.
    _groups->group_array[group_num].alive = false;
//
    // remove object from the mapping
    _obj_to_group_num.erase(obj);
    
    std::cout <<"removed mapping\n";
}

/**
 * Goes through all of the ParticleWrapper we have, and calls update on each one of them
 */
void PulseParticleGenerator::generate() {
    if (!_active) return;
    
    for (auto it = _obj_to_group_num.begin(); it != _obj_to_group_num.end(); it++) {
        GameObject* obj = it->first;
        int group_num = it->second;
        
        // sync the position of the group to the character
        Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
        _groups->group_array[group_num].global_position = world_pos;
    }
    
    // update every particle in this node
    _pulsepartnode->update();
}






