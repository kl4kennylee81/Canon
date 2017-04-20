//
//  ParticleData.hpp
//  MemoryDemo
//
//  Created by Hong Jeon on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ParticleData_hpp
#define ParticleData_hpp

#include <stdio.h>
#include "cugl/cugl.h"

using namespace cugl;

/**
 * Contain particle information that can be loaded in from a data file.
 * The properties are public on purpose because it is inherently
 * simple enough to manipulate without getters and setters.
 */
class ParticleData {
public:
    Vec2 position;
    Vec2 velocity;
    Vec2 gravity;
    
    float acceleration;
    
    /** Time to live is counted by the number of frames */
    int ttl;
    
    bool active;
    
    Color4f start_color;
    Color4f end_color;
    /** How long it takes for the color to transition from start to end */
    int color_duration;
    
    
    ParticleData() {}
    
    bool init(Vec2 p, Vec2 v, Vec2 g, float a, int tl, bool ac, Color4f sc, Color4f ec, int cd) {
        position = p;
        velocity = v;
        gravity = g;
        acceleration = a;
        ttl = tl;
        active = ac;
        start_color = sc;
        end_color = ec;
        color_duration = cd;
        return true;
    }
    
    void reset() {
        position.setZero();
        velocity.setZero();
        gravity.setZero();
        acceleration = 0;
        ttl = 0;
        active = false;
        start_color = Color4f();
        end_color = Color4f();
        color_duration = 0;
    }
};

#endif /* ParticleData_hpp */
