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

#pragma mark -
#pragma mark Particle

bool Particle::init(ParticleData pd) {
    _pd = pd;
    _time_alive = 0;
    
    if (pd.color_fade) {
        _current_color = _pd.start_color;
        _color_step = findColorStep();
    }
    
    if (pd.alpha_fade) {
        _alpha_step = findAlphaStep();
        _current_color.set(_current_color.r, _current_color.g, _current_color.b, pd.start_alpha);
    }
    
    return true;
}

void Particle::move() {
    
    if (!_pd.active) return;
    
    if (_pd.move) {
        // iteratively simulating kinematics
        _pd.position += _pd.velocity;
        _pd.velocity.x += _pd.velocity.x*_pd.acceleration*_time_alive;
        _pd.velocity.y += _pd.velocity.y*_pd.acceleration*_time_alive;
        _pd.velocity += _pd.gravity*_time_alive;
    }
    
    if (_pd.color_fade) {
        // take into account color interpolation. Linear for now.
        _current_color.add(_color_step, false);
    }
    
    if (_pd.alpha_fade) {
        _current_color.subtract(_alpha_step, true);
    }
    
    if (_pd.scale) {
        _pd.current_scale += findScaleStep();
    }
    
    if (_pd.rotate) {
        _pd.current_angle += findAngularVelocity(_pd.revolution_time);
    }
    
    if (_pd.ttl == 0) {
        _pd.active = false;
    }
    
    _pd.ttl--;
    _time_alive++;
}

float Particle::findScaleStep() {
    return (((float)_pd.end_scale-_pd.current_scale)/_pd.ttl);
}

Color4f Particle::findColorStep() {
    if (_pd.color_duration == -1) {
        return Color4f(0,0,0,0);
    }
    
    Color4f diff = _pd.end_color.subtract(_pd.start_color);
    diff.scale(1/((float)_pd.color_duration));
    return diff;
}

Color4f Particle::findAlphaStep() {
    float step = _pd.start_alpha / (float)_pd.alpha_duration;
    Color4f diff = Color4f(0, 0, 0, step);
    return diff;
}

float Particle::findAngularVelocity(float time) {
    return M_PI_4/time;
}

bool Particle::isActive() {
    return _pd.active;
}


void Particle::reset() {
    _time_alive = 0;
    _pd.reset();
}
