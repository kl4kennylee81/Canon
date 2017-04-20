//
//  ParticleController.cpp
//  Canon
//
//  Created by Hong Jeon on 4/10/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleController.hpp"
#include "PathEvent.hpp"
#include "InputController.hpp"
#include "GameState.hpp"
#include <random>

using namespace cugl;

#define MAX_PARTICLES 100

ParticleController::ParticleController(): BaseController(){}

void ParticleController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void ParticleController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void ParticleController::notify(Event* e) {
    BaseController::notify(e);
}
void ParticleController::eventUpdate(Event* e) {
}

void ParticleController::update(float timestep, std::shared_ptr<GameState> state) {
    
}

bool ParticleController::init() {
    _memory = FreeList<Particle>::alloc(MAX_PARTICLES);
    _cooldown = 0;
    return true;
}


