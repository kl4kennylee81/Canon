//
//  PathParticleGenerator.cpp
//  Canon
//
//  Created by Hong Jeon on 5/19/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PathParticleGenerator.hpp"
#include "Util.hpp"

#define PARTICLE_NUM 2
#define MAX_PARTICLES 500
#define MAX_GROUPS 2
#define FADE_TIME 40
#define MIN_DISTANCE 0.2
#define THRESH 0.2

bool PathParticleGenerator::init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
    _active = false; // default
    _particle_map = particle_map;
    _groups = GroupContainer::alloc(MAX_GROUPS);
    
    // don't use group positioning. custom init for now. hacky
    for (int i = 0; i < MAX_GROUPS; i++) {
        _groups->group_array[i].position = false;
        _groups->group_array[i].repeat = false;
        _groups->group_array[i].fade_time = FADE_TIME;
        _groups->group_array[i].alive = true;
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
    _last_size = 0;
    
    return true;
}

int element_to_group(ElementType element) {
    int group_num;
    if (element == ElementType::BLUE) {
        group_num = 0;
    } else {
        group_num = 1;
    }
    
    return group_num;
}

void PathParticleGenerator::add_path(std::shared_ptr<Path> path, ElementType element, Vec2 starting_point) {
    int group_num = element_to_group(_active_element);
    _groups->group_array[group_num].fade = false;
    _last_size = 0;
    _last_point = starting_point;
    _path = path; // no copy;
    _active_element = element;
}

void PathParticleGenerator::remove_path(GameObject* active_char) {
    int group_num = element_to_group(_active_element);
    _groups->group_array[group_num].fade = true;
}

void PathParticleGenerator::add_particles(int group_num, std::vector<Vec2> locations) {
    ParticleData pd;
    
    if (group_num == 0) {
        pd = _blue_path_pd;
    } else {
        pd = _gold_path_pd;
    }
    
    for (auto it = locations.begin(); it != locations.end(); it++) {
        // sync positions
        pd.position = (*it)*Util::getGamePhysicsScale();
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
    Vec2 first;
    
    // gotta wait until path has at least two points
    if (_path->size() <= 1) {
        return;
    }
    
    /*
    // if its the first point, then use the first point as last also
    if (_last_size == 0) {
        first = _path->getLast();
    } else {
        // otherwise take the last point in the path
        first = _last_point;
    }*/

    first = _last_point;
    
    // generate the vector of the difference between last point and path
    std::vector<Vec2> locations = interpolate(first, _path->getLast(), MIN_DISTANCE, THRESH);

    // add particles on the difference
    add_particles(group_num, locations);
    
    _last_size = _path->size();
    _last_point = _path->getLast();
}

void PathParticleGenerator::generate(std::shared_ptr<GameState> state) {
    if (!_active) return;

    int group_num = element_to_group(_active_element);
    update_paths(group_num);

    _bluepartnode->update();
    _goldpartnode->update();
}
