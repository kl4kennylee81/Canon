//
//  ParticleGenerator.cpp
//  MemoryDemo
//
//  Created by Hong Jeon on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleGenerator.hpp"

float ParticleGenerator::getRandomFloat(float a, float b) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(a, b);
    float rand = dist(mt);
    return rand;
}
