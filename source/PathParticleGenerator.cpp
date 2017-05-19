//
//  PathParticleGenerator.cpp
//  Canon
//
//  Created by Hong Jeon on 5/19/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PathParticleGenerator.hpp"

#define PARTICLE_NUM 2
#define MAX_PARTICLES 200
#define MAX_GROUPS 2
#define FADE_TIME 40

bool PathParticleGenerator::init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _active = false; // default
    _particle_map = particle_map;
    _groups = GroupContainer::alloc(MAX_GROUPS);
    
    // don't use group positioning
    for (int i = 0; i < MAX_GROUPS; i++) {
        _groups->group_array[i].position = false;
        _groups->group_array[i].fade_time = FADE_TIME;
    }
    
    // initialize particle node and attach to the world node
    _blue_path_pd = _particle_map->at("blue_path");
    _bluepartnode = ParticleNode::allocWithTexture(_blue_path_pd.texture);
    _bluepartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _bluepartnode->setBlendEquation(GL_FUNC_ADD);
    _bluepartnode->setPosition(Vec2::ZERO);
    _bluepartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    
    // important steps
    _bluepartnode->init_memory(MAX_PARTICLES);
    _bluepartnode->_groups = _groups;
    state->getWorldNode()->addChild(_bluepartnode);
    
    _gold_path_pd = _particle_map->at("gold_path");
    _goldpartnode = ParticleNode::allocWithTexture(_gold_path_pd.texture);
    _goldpartnode->setBlendFunc(GL_SRC_ALPHA, GL_ONE);
    _goldpartnode->setBlendEquation(GL_FUNC_ADD);
    _goldpartnode->setPosition(Vec2::ZERO);
    _goldpartnode->setAnchor(Vec2::ANCHOR_MIDDLE);
    
    // important steps
    _goldpartnode->init_memory(MAX_PARTICLES);
    _goldpartnode->_groups = _groups;
    state->getWorldNode()->addChild(_goldpartnode);
    
    
    // instance variables
    _last_size = -1;
    _last_point = Vec2(0,0);
    
    return true;
}

void PathParticleGenerator::add_path(std::shared_ptr<Path> path) {
    _path = path; // no copy;
}

void PathParticleGenerator::add_particles(int group_num, std::vector<Vec2> locations) {
    ParticleData pd;
    
    if (group_num == 0) {
        pd = _blue_path_pd;
    } else {
        pd = _gold_path_pd;
    }
    
    for (auto it = locations.begin(); it != locations.end(); it++) {
        pd.position = (*it);
        if (group_num == 0) {
            _bluepartnode->addParticle(pd, group_num, pd);
        } else {
            _goldpartnode->addParticle(pd, group_num, pd);
        }
    }
}

/**
 * Sync paths and add particles on the new points that came in the path
 */
void PathParticleGenerator::update_paths(int group_num) {
    // pathcontroller has reset the path
    if (_path->size() < _last_size) {
        _groups->group_array[group_num].fade = true;
    }
    else {
        // generate the vector of the difference between last point and path
        std::vector<Vec2> locations;
        
        // add particles on the difference
        add_particles(group_num, locations);
    }
    
    _last_size = _path->size();
    _last_point = _path->getLast();
}

void PathParticleGenerator::generate(std::shared_ptr<GameState> state) {
    if (!_active) return;
    
    ElementType active_element = state->getActiveCharacter()->getPhysicsComponent()->getElementType();
    int group_num;
    if (active_element == ElementType::BLUE) {
        group_num = 0;
    } else {
        group_num = 1;
    }
    update_paths(group_num);

    _bluepartnode->update();
    _goldpartnode->update();
}
