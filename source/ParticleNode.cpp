//
//  ParticleNode.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleNode.hpp"

void ParticleNode::addParticle(ParticleData pd, int g_num) {
    Particle* particle = _memory->malloc();
    if (particle != nullptr) {
        // set the attributes of the new particle
        particle->init(pd);
        particle->group_num = g_num;
    }
}

void ParticleNode::update() {
    std::set<Particle*> to_remove;
    
    for (int i = 0; i < _memory->_capacity; i++) {
        Particle* particle = &_memory->_prealloc[i];
        
        // skip over ones that have been freed
        if (!particle->_pd.active) {
            continue;
        }
        
        // find out the particle groupnum and update the groups
        group_update_particles(particle);
        
        particle->move();
        
        if (!particle->isActive()) {
            to_remove.insert(particle);
        }
    }
    
    // free the ones that aren't active anymore
    for (auto it = to_remove.begin(); it != to_remove.end(); it++) {
        _memory->free((*it));
    }
}

void ParticleNode::group_update_particles(Particle* particle) {
    // don't update group if particle doesn't have a group
    if (particle->group_num < 0) {
        return;
    }
    // find the group
    Group g = _groups->group_array[particle->group_num];
    // update position
    particle->_pd.position = g.global_position;
    // update active based on group
    particle->_pd.active = g.alive;
}

Mat4 ParticleNode::updateTransformLocal(Mat4 combined, float scale, float angle) {
    Vec2 offset = _anchor*getContentSize();
    
    Mat4::createTranslation(-offset.x, -offset.y, 0.0f, &combined);
    combined.scale(scale, scale, 1.0f);
    combined.rotateZ(angle);
    combined.translate(offset.x, offset.y, 0.0f);
    
    combined.m[12] += _position.x-offset.x;
    combined.m[13] += _position.y-offset.y;
    
    return combined;
}

/**
 * This is our custom draw code that draws all the nodes in the ParticleNode by moving around
 * a rectangle box and calling batch->fill() on it.
 */
void ParticleNode::draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) {
    batch->setTexture(_texture);
    batch->setBlendEquation(_blendEquation);
    batch->setBlendFunc(_srcFactor, _dstFactor);
    Mat4 combined = this->getCombined();
    
    // loop through the entire memory
    for (int i = 0; i < _memory->_capacity; i++) {
        Particle particle = _memory->_prealloc[i];
        
        // if not active, don't draw
        if (!particle._pd.active) {
            continue;
        }
        
        Mat4 new_transform;
        
        if (particle._pd.scale) {
            new_transform = updateTransformLocal(combined, particle._pd.current_scale, particle._pd.current_angle);
        }
        
        Rect bounds = _polygon.getBounds();
        bounds.origin += particle._pd.position;
        batch->setColor(particle.getCurrentColor());
        batch->fill(bounds, bounds.origin, new_transform);
    }
}
