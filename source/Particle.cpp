//
//  Particle.cpp
//  MemoryDemo
//
//  This is a subclass of PolygonNode to represent a particle.
//
//  Author: Walker White and Anthony Perello
//  Version: 2/17/17
//
#include "Particle.h"


void Particle::move() {
    // iteratively simulating kinematics
    _pd.position += _pd.velocity;
    _pd.velocity.x += _pd.velocity.x*_pd.acceleration*_time_alive;
    _pd.velocity.y += _pd.velocity.y*_pd.acceleration*_time_alive;
    _pd.velocity += _pd.gravity*_time_alive;
    
    // take into account color interpolation. Linear for now.
    _current_color.add(_color_step, false);
    
    // take into account ttl fades
    _current_color.subtract(_alpha_step, true);
    
    if (_current_color.a == 0) {
        _pd.active = false;
    }
    
    _time_alive++;
}

Color4f Particle::findColorStep() {
    Color4f diff = _pd.end_color.subtract(_pd.start_color);
    diff.scale(1/((float)_pd.color_duration));
    return diff;
}

Color4f Particle::findAlphaStep() {
    Color4f diff = Color4f(0, 0, 0, 1/(float)_pd.ttl);
    return diff;
}

bool Particle::isActive() {
    return _pd.active;
}


void Particle::reset() {
    _pd.reset();
}


/**
 * Updates all of the particles, moving them one time step
 *
 * If a particle goes out of the bounding box of this node, it is added
 * to the reset set.  This allows the caller a chance to remove and
 * recycle these particles.
 *
 * @param reset The recycle set
 */

void ParticleNode::update(std::set<Particle*>& reset) {
    Rect bounds(Vec2::ZERO, getContentSize());
    for (auto it=_particles.begin(); it !=_particles.end(); ++it) {
        Particle* p = *it;
        p->move();
        
        if (!p->isActive()) {
            reset.insert(p);
        }
    }
}

/**
 * Draws this Node via the given SpriteBatch.
 *
 * This method only worries about drawing the current node.  It does not
 * attempt to render the children.
 *
 * This is the method that you should override to implement your custom
 * drawing code.  You are welcome to use any OpenGL commands that you wish.
 * You can even skip use of the SpriteBatch.  However, if you do so, you
 * must flush the SpriteBatch by calling end() at the start of the method.
 * in addition, you should remember to call begin() at the start of the
 * method.
 *
 * This method provides the correct transformation matrix and tint color.
 * You do not need to worry about whether the node uses relative color.
 * This method is called by render() and these values are guaranteed to be
 * correct.  In addition, this method does not need to check for visibility,
 * as it is guaranteed to only be called when the node is visible.
 *
 * @param batch     The SpriteBatch to draw with.
 * @param matrix    The global transformation matrix.
 * @param tint      The tint to blend with the Node color.
 */
void ParticleNode::draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) {
    batch->setTexture(_texture);
    //    batch->setBlendEquation(_blendEquation);
    //    batch->setBlendFunc(_srcFactor, _dstFactor);
    
    for(auto it = _particles.begin(); it != _particles.end(); ++it) {
        Rect bounds = _polygon.getBounds();
        bounds.origin += (*it)->_pd.position-bounds.size/2.0f;
        batch->setColor((*it)->getCurrentColor());
        batch->fill(bounds);
    }
}

