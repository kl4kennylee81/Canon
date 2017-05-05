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
#include "CollisionEvent.hpp"

using namespace cugl;

#define MAX_PARTICLES 1000

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
    switch (e->_eventType) {
        case Event::EventType::COLLISION: {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    GameObject* obj = objectGone->_obj;
                    handleDeathParticle(obj);
                    break;
            }
            break;
        }
    }
}

void ParticleController::handleDeathParticle(GameObject* obj) {
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    _death_gen->add_particles(world_pos, obj->getPhysicsComponent()->getElementType());
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
    
    /** Particle for the Trail */
    ParticleData pd = ParticleData();
    pd.move = true;
    pd.acceleration = -0.005;
    pd.ttl = 40;
    pd.color_fade = true;
    pd.start_color = Color4f::WHITE;
    pd.end_color = Color4f::WHITE;
    pd.color_duration = -1;
    pd.alpha_fade = true;
    pd.alpha_duration = 40;
    pd.start_alpha = 1;
    pd.texture_name = "blue_particle";
    pd.texture = assets->get<Texture>("blue_particle");
    
    ParticleData pd2 = ParticleData();
    pd2.move = true;
    pd2.acceleration = -0.005;
    pd2.ttl = 40;
    pd2.start_color = Color4f::WHITE;
    pd2.end_color = Color4f::WHITE;
    pd2.color_duration = -1;
    pd2.alpha_fade = true;
    pd2.alpha_duration = 40;
    pd2.start_alpha = 1;
    pd2.texture_name = "gold_particle";
    pd2.texture = assets->get<Texture>("gold_particle");
    
    /** Blue Death particle */
    ParticleData temp;
    temp.color_fade = true;
    temp.start_color = Color4f::WHITE;
    temp.end_color = Color4f::WHITE;
    temp.color_duration = -1; // infinite
    temp.move = true;
    temp.gravity = Vec2(0,0);
    temp.acceleration = -0.005;
    temp.alpha_fade = true;
    temp.alpha_duration = 50;
    temp.start_alpha = 1;
    temp.rotate = true;
    temp.current_angle = 0;
    temp.revolution_time = 2;
    temp.scale = true;
    temp.start_scale = 0.3;
    temp.end_scale = 0.1;
    temp.current_scale = 0.5;
    temp.texture_name = "blue_death";
    temp.texture = assets->get<Texture>(temp.texture_name);
    
    ParticleData temp2 = temp;
    temp2.texture_name = "green_death";
    temp2.texture = assets->get<Texture>(temp2.texture_name);
    
    
    _particle_map.insert(std::make_pair(pd.texture_name, pd)); // blue trail
    _particle_map.insert(std::make_pair(pd2.texture_name, pd2)); // gold trail
    _particle_map.insert(std::make_pair(temp.texture_name, temp)); // blue death
    _particle_map.insert(std::make_pair(temp2.texture_name, temp)); // gold death
    
    _death_gen = DeathParticleGenerator::alloc(_memory, pd, state, &_particle_map);
    _death_gen->start();
    
    return true;
}

