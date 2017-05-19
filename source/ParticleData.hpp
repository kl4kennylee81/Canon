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
 *
 * IMPORTANT: set the following flags you want: move, scale, rotate, color_fade, alpha_fade, and
 * all the properties underneath those flags. All will default to false;
 *
 */
class ParticleData {
public:
    /**
     * Any particle that is not active is removed from the node and reset() in memory
     */
    bool active;
    int ttl; // -1 means infinite
    std::string texture_name;
    std::shared_ptr<Texture> texture;
    
    /**
     * Movement
     */
    bool move;
    Vec2 position;
    Vec2 velocity;
    Vec2 gravity;
    float acceleration;
    
    /**
     * Scale
     */
    bool scale;
    float start_scale; // scale when time_alive = 0
    float end_scale; // sacle when time_alive = ttl
    float current_scale;
    
    /**
     * Rotate
     */
    bool rotate;
    float current_angle;
    float revolution_time; // # frames for a full revolution
    
    /**
     * Color fade
     */
    bool color_fade;
    int color_duration; // # frames till end_color
    Color4f start_color;
    Color4f end_color;
    
    /**
     * Alpha fade
     */
    bool alpha_fade;
    int alpha_duration; // # frames till end_alpha
    float start_alpha;
    
    bool group_fade;
    
    /**
     * TODO: Everything undearneath this point needs to get completed in order for JSON loading.
     */
    
    ParticleData() {
        active = true;
        ttl = -1;
        current_angle = 0;
        
        // default flags
        move = false;
        scale = false;
        rotate = false;
        color_fade = false;
        alpha_fade = false;
    }
    
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
        active = false;
        start_color = Color4f();
        end_color = Color4f();
        color_duration = 0;
        ttl = -1;
        current_angle = 0;
        
        move = false;
        scale = false;
        rotate = false;
        color_fade = false;
        alpha_fade = false;
    }
};

#endif /* ParticleData_hpp */
