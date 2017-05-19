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
#define PULSE_RATE 3
#define NUM_PARTICLES 3
// max number of pulse particles
#define MAX_PARTICLES 40*PULSE_RATE*NUM_PARTICLES
#define MAX_GROUPS 1000

#define BLUER 49
#define BLUEG 185
#define BLUEB 255

#define GOLDR 235
#define GOLDG 235
#define GOLDB 56


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
void PulseParticleGenerator::createPulseParticles(Vec2 world_pos, int group_num, ElementType element) {
    ParticleData pd = _ringpd; // don't taint the template
    
    // how much scale each pulse is separated by
    float scale_rate = ((float)(pd.end_scale - pd.start_scale)/(PULSE_RATE-1));
    // how much ttl each pulse is separated by
    float ttl_rate = ceil(((float)pd.ttl)/(PULSE_RATE-1));
    pd.current_scale = pd.start_scale;
    
    if (element == ElementType::BLUE) {
        pd.color_fade = true;
        pd.start_color = normalizedRGB(BLUER,BLUEG,BLUEB,1);
        pd.end_color = normalizedRGB(BLUER,BLUEG,BLUEB,1);
        pd.color_duration = -1; // infinite
    } else {
        pd.color_fade = true;
        pd.start_color = normalizedRGB(GOLDR,GOLDG,GOLDB,1);
        pd.end_color = normalizedRGB(GOLDR,GOLDG,GOLDB,1);
        pd.color_duration = -1; // infinite
    }
    
    // pd is the original starting particle
    ParticleData original = pd;
    
    // create the particles that are spaced out by a constant amount
    for (int ii = 0; ii < PULSE_RATE; ii++) {
        _pulsepartnode->addParticle(randomizeAngle(pd), group_num, original);
        _pulsepartnode->_original = original;
        pd.current_scale += scale_rate; // ones in the outer ring are bigger
        pd.ttl -= ttl_rate; // ones in the outer ring die sooner
    }
}

void PulseParticleGenerator::add_mapping(GameObject* obj) {
    // don't add pulses on players or zones or bullets
    if (obj->type != GameObject::ObjectType::CHARACTER || obj->getIsPlayer()) return;
    
    // this might be the source of the gravity bug?
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    bool repeat = true;
    
    // finds the next available group num
    int group_num = _groups->makeNewGroup(world_pos, repeat);
    ElementType element = obj->getPhysicsComponent()->getElementType();
    
    // create the wrapper
    for (int i = 0; i < NUM_PARTICLES; i++) {
        createPulseParticles(world_pos, group_num, element);
    }
    
    // insert wrapper to object -> wrapper map
    _obj_to_group_num.insert(std::make_pair(obj, group_num));
}

void PulseParticleGenerator::remove_mapping(GameObject* obj) {
    if (obj->type != GameObject::ObjectType::CHARACTER || obj->getIsPlayer()) return;
    
    // find out what group the object belongs to
    auto group_num = _obj_to_group_num.at(obj);
    
    // mark this one as done. very important.
    _groups->group_array[group_num].alive = false;

    // remove object from the mapping
    _obj_to_group_num.erase(obj);
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






