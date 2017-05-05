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
#include <memory>
#include <assert.h>

using namespace cugl;

#define MAX_PARTICLES 300

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
    // fill in trailparticle generator map if not initialized yet. This initialization is
    // here instead of in init() because the player chars have not been initialized at init().
    if (_character_map.size() == 0) {
        
        for (std::shared_ptr<GameObject> gameObj : state->getPlayerCharacters()){
            ParticleData pd;
            if (gameObj->getPhysicsComponent()->getElementType() == ElementType::GOLD) {
                pd = _particle_map.at("gold_particle");
            } else {
                pd = _particle_map.at("blue_particle");
            }
            
            std::shared_ptr<TrailParticleGenerator> generator = TrailParticleGenerator::alloc(_memory, pd, gameObj, state);
            generator->start(); // replace this with events later?
            _character_map.insert(std::make_pair(gameObj.get(), generator));
        }
    }
    
    generateTrails();
}

void ParticleController::generateTrails() {
    for (auto it = _character_map.begin(); it != _character_map.end(); it++) {
        std::shared_ptr<TrailParticleGenerator> obj = it->second;
        obj->generate();
    }
}

bool ParticleController::init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
    _memory = FreeList<Particle>::alloc(MAX_PARTICLES);
    
    // init the different types of particles
    // replace this with json loading later.
    ParticleData pd = ParticleData();
    pd.acceleration = -0.005;
    pd.ttl = 40;
    pd.start_color = Color4f::WHITE;
    pd.end_color = Color4f::WHITE;
    pd.color_duration = 100;
    pd.active = true;
    pd.texture_name = "blue_particle";
    pd.texture = assets->get<Texture>("blue_particle");
    _particle_map.insert(std::make_pair(pd.texture_name, pd));
    
    ParticleData pd2 = ParticleData();
    pd2.acceleration = -0.005;
    pd2.ttl = 40;
    pd2.start_color = Color4f::WHITE;
    pd2.end_color = Color4f::WHITE;
    pd2.color_duration = 100;
    pd2.active = true;
    pd2.texture_name = "gold_particle";
    pd2.texture = assets->get<Texture>("gold_particle");
    _particle_map.insert(std::make_pair(pd2.texture_name, pd2));
    
    return true;
}

