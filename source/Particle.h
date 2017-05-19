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
    int _time_alive;
    
    /** These are fields that you do not set on initialization. */
    Color4f _color_step;
    Color4f _current_color;
    Color4f _alpha_step;
    
    /**
     * Returns a Color4f object that can be added to the current color at each step
     */
    Color4f findColorStep();
    Color4f findAlphaStep();
    float findScaleStep();
    
public:
    // this is the data it starts from if we reset the particles. like in pulse
    ParticleData _original;
    
    /** Holds all properties of the particle that are set by a data file */
    ParticleData _pd;
    
    Particle() {
        group_num = -1;
    }
    
    bool init(ParticleData pd);
    
    /**
     * Performs one step of frame of the particle.
     */
    void move();
    
    /** Must have this method to be compatible with a FreeList */
    void reset();
    
    /**
     * Uses the particle data from when it was initialized.
     * Instance variables are initialized too.
     */
    void replay_from_beginning();
    
    Color4f getCurrentColor() { return _current_color; }
    
    bool isActive();
    
    float findAngularVelocity(float time);
    
    int group_num;
};

#endif /* defined(__MD_PARTICLE_H__) */
