//
//  Particle.cpp
//  MemoryDemo
//
//  This file provides a pair of classes: a Particle and a ParticleNode.  The
//  ParticleNode is used to draw the Particle as part of a scene graph.  For
//  efficient reasons, you have many Particles, but only one ParticleNode.
//
//  Author: Walker White and Anthony Perello
//  Version: 2/17/17
//
#ifndef __MD_PARTICLE_H__
#define __MD_PARTICLE_H__

#include "ParticleData.hpp"
#include "cugl/cugl.h"
#include <set>
#include <cmath>

using namespace cugl;

/** How many pixels the particle moves per animation frame */
#define PARTICLE_SPEED 1.0f

#pragma mark -
#pragma mark Particle

/**
 * A general particle class whose data must be initialized by ParticleData.
 */
class Particle {
private:
    /** These are fields that you do not set on initialization. */
    
    /** Internal clock for the particle */
    int _time_alive;
    Color4f _color_step;
    Color4f _current_color;
    Color4f _alpha_step;
    
    /**
     * Returns a Color4f object that can be added to the current color at each step
     */
    Color4f findColorStep();
    Color4f findAlphaStep();
    
public:
    /** Holds all properties of the particle that are set by a data file */
    ParticleData _pd;
    
    Particle() {}
    
    bool init(ParticleData pd) {
        _pd = pd;
        _time_alive = 0;
        _color_step = findColorStep();
        _current_color = _pd.start_color;
        _alpha_step = findAlphaStep();
        return true;
    }
    
    /**
     * Performs one step of frame of the particle.
     */
    void move();
    
    /** Must have this method to be compatible with a FreeList */
    void reset();
    
    Color4f getCurrentColor() { return _current_color; }
    
    bool isActive();
};


#pragma mark -
#pragma mark ParticleNode
/**
 * A scene graph node for a list of particles.
 *
 * For this demo, we do not need any physics.  It is fine enough to draw.  We
 * only need one ParticleNode for a collection of particles.
 */
class ParticleNode : public PolygonNode {
private:
    /** Container to store the particles currently allocated */
    std::set<Particle*> _particles;
    
public:
    /**
     * Returns a new ParticleNode with the given texture
     *
     * All particles will use the given texture as their representation. The
     * particles will have the natural size of this texture; individual particles
     * cannot be scaled.  For more control, you will need to implement your
     * own particle node.
     *
     * @param texture   A shared pointer to a Texture object.
     *
     * @return a new ParticleNode with the given texture
     */
    static std::shared_ptr<ParticleNode> allocWithTexture(const std::shared_ptr<Texture>& texture) {
        std::shared_ptr<ParticleNode> node = std::make_shared<ParticleNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }
    
    /**
     * Returns the number of particles in this node
     *
     * @return the number of particles in this node
     */
    size_t getParticleCount() { return _particles.size(); }
    
    /**
     * Adds a particle to this node
     *
     * If the particle is already in the node, nothing happens
     *
     * @param particle  The particle to add to this node
     */
    void addParticle(Particle* particle) {
        _particles.insert(particle);
    }
    
    /**
     * Removes a particle from this node
     *
     * If the particle is not in the node, nothing happens
     *
     * @param particle  The particle to remove from this node
     */
    void removeParticle(Particle* particle) {
        _particles.erase(particle);
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
    void update(std::set<Particle*>& reset);
    
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
     * @param transform The global transformation matrix.
     * @param tint      The tint to blend with the Node color.
     */
    virtual void draw(const std::shared_ptr<SpriteBatch>& batch, const Mat4& transform, Color4 tint) override;
    
};

#endif /* defined(__MD_PARTICLE_H__) */
