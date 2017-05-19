//
//  ParticleNode.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleNode.hpp"
#include "Util.hpp"

void ParticleNode::addParticle(ParticleData pd, int g_num, ParticleData original) {
    Particle* particle = _memory->malloc();
    if (particle != nullptr) {
        // set the attributes of the new particle
        particle->init(pd);
        particle->group_num = g_num;
        particle->_original = original;
    }
}

void ParticleNode::group_update_particles(Particle* particle) {
    // don't update group if particle doesn't have a group
    if (particle->group_num < 0) {
        return;
    }
    
    // find the group
    Group* g = &_groups->group_array[particle->group_num];
    
    if (g->position) {
        particle->_pd.position = g->global_position;
    }
    
    if (g->fade) {
        // check if fade on the particle is already happening
        if (!particle->_pd.alpha_fade) {
            particle->_pd.alpha_fade = true;
            // change from infinite to set time limit
            particle->_pd.ttl = particle->_pd.alpha_duration;
        }
    }
    
    // update active based on group
    particle->_pd.active = g->alive;
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
            // check if this guy is supposed to repeat
            if (particle->group_num >= 0 && _groups->group_array[particle->group_num].repeat) {
                particle->replay_from_beginning();
            } else {
                to_remove.insert(particle);
            }
        }
    }
    
    // free the ones that aren't active anymore
    for (auto it = to_remove.begin(); it != to_remove.end(); it++) {
        _memory->free((*it));
    }
}

Mat4 ParticleNode::updateTransformLocal(Mat4 combined, float scale, float angle) {
    Vec2 offset = _anchor*getContentSize();
    
    // Hacky taking into account the offset of the cloud/black border on top and bottom
    // the worldYTranslate is in scene node coordinates (1024 x ?)
    
    Mat4::createTranslation(-offset.x, -offset.y, 0.0f, &combined);
    combined.scale(scale, scale, 1.0f);
    combined.rotateZ(angle);
    combined.translate(offset.x, offset.y, 0.0f);
    
    combined.m[12] += _position.x-offset.x;
    combined.m[13] += _position.y-offset.y + Util::getSceneToWorldTranslateY();
    
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
