//
//  ParticleNode.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleNode.hpp"
#include "Util.hpp"

void ParticleNode::update(std::set<Particle*>& reset) {
    for (auto it=_particles.begin(); it !=_particles.end(); ++it) {
        Particle* p = *it;
        p->move();
        
        if (!p->isActive()) {
            reset.insert(p);
        }
    }
}

Mat4 ParticleNode::updateTransformLocal(Mat4 combined, float scale, float angle) {
    Vec2 offset = _anchor*getContentSize();
    
    // Hacky taking into account the offset of the cloud/black border on top and bottom
    offset-= Util::getWorldTranslateY();
    
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
    
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Mat4 new_transform;
        
        if ((*it)->_pd.scale) {
            new_transform = updateTransformLocal(combined, (*it)->_pd.current_scale, (*it)->_pd.current_angle);
        }
        
        Rect bounds = _polygon.getBounds();
        bounds.origin += (*it)->_pd.position;
        batch->setColor((*it)->getCurrentColor());
        batch->fill(bounds, bounds.origin, new_transform);
    }
}
