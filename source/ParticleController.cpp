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
#include "MoveEvent.hpp"

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
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT: {
                    ObjectInitEvent* objectInit = (ObjectInitEvent*)levelEvent;
                    addObject(objectInit->object.get(), objectInit->partStateData);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWNING: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    handleAction(obj, AnimationAction::SPAWNING);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    handleAction(obj, AnimationAction::SPAWN);
                    break;
                }
            }
            break;
        }
        case Event::EventType::COLLISION: {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    GameObject* obj = objectGone->_obj;
                    handleAction(obj, AnimationAction::DEATH);
                    // do something to remove from mapping
                    //animationMap.at(obj)->setLastAnimation();
                    break;
            }
            break;
        }
        case Event::EventType::PATH: {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
                case PathEvent::PathEventType::PATH_FINISHED:
                    PathFinished* pathFinished = (PathFinished*)pathEvent;
                    GameObject* obj = pathFinished->_activeChar.get();
                    handleAction(obj, AnimationAction::ATTACK);
                    break;
            }
            break;
        }
        case Event::EventType::MOVE: {
            MoveEvent* moveEvent = (MoveEvent*)e;
            switch(moveEvent->_moveEventType){
                case MoveEvent::MoveEventType::MOVE_FINISHED:
                {
                    GameObject* obj = moveEvent->_character.get();
                    handleAction(obj, AnimationAction::RETURN);
                    break;
                }
            }
            break;
        }
        case Event::EventType::ZONE: {
            ZoneEvent* zoneEvent = (ZoneEvent*)e;
            switch (zoneEvent->zoneEventType) {
                case ZoneEvent::ZoneEventType::ZONE_INIT: {
                    ZoneInitEvent* zoneInit = (ZoneInitEvent*)zoneEvent;
                    addObject(zoneInit->object.get(), zoneInit->partStateData);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_SPAWNING: {
                    ZoneSpawningEvent* zoneSpawning = (ZoneSpawningEvent*)zoneEvent;
                    handleAction(zoneSpawning->object, AnimationAction::SPAWNING);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_SPAWN: {
                    ZoneSpawnEvent* zoneSpawn = (ZoneSpawnEvent*)zoneEvent;
                    handleAction(zoneSpawn->object, AnimationAction::SPAWN);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_ON: {
                    ZoneOnEvent* zoneOn = (ZoneOnEvent*)zoneEvent;
                    GameObject* obj = zoneOn->object;
                    handleAction(obj, AnimationAction::ATTACK);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_OFF: {
                    ZoneOffEvent* zoneOff = (ZoneOffEvent*)zoneEvent;
                    GameObject* obj = zoneOff->object;
                    handleAction(obj, AnimationAction::ATTACK);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_DELETE: {
                    ZoneDeleteEvent* zoneDelete = (ZoneDeleteEvent*)zoneEvent;
                    GameObject* obj = zoneDelete->object;
                    handleAction(obj, AnimationAction::DEATH);
                    // do something to remove from mapping
                    //animationMap.at(obj)->setLastAnimation();
                    break;
                }
            }
            break;
        }
    }
}

void ParticleController::addObject(GameObject* obj, std::shared_ptr<ParticleStateData> psd){
    std::shared_ptr<ActiveParticleState> part = ActiveParticleState::alloc();
    part->setParticleStateData(psd);
    objectStateMap.insert({obj,part});
}

void ParticleController::handleAction(GameObject* obj, AnimationAction animAction){
    if (obj == nullptr){
        return;
    }
    std::shared_ptr<ActiveParticleState> partState = objectStateMap.at(obj);
    if (partState == nullptr){
        return;
    }
    
    //if animation action not in map, don't do anything
    if (!(partState->hasAnimationAction(animAction))){
        return;
    }
    
    //stop these generators
    std::vector<ParticleAction> actionsToStop = partState->getCurrentParticleActions();
    for (auto pa : actionsToStop){
        switch (pa) {
            case ParticleAction::PULSE:
                _pulse_gen->remove_mapping(obj);
                break;
            case ParticleAction::TRAIL:
                _trail_gen->remove_character(obj);
                break;
            case ParticleAction::ZONE:
//                _zone_gen->remove_mapping(obj);
                break;
            default:
                break;
        }
    }
    
    partState->setAnimationAction(animAction);
    
    //start these generators
    std::vector<ParticleAction> actionsToStart = partState->getCurrentParticleActions();
    for (auto pa : actionsToStart){
        switch (pa) {
            case ParticleAction::PULSE:
                _pulse_gen->add_mapping(obj);
                break;
            case ParticleAction::TRAIL:{
                _trail_gen->add_character(obj);
                _trail_gen->start();
                break;
            }
            case ParticleAction::ZONE:{
//                _zone_gen->add_mapping(obj);
                break;
            }
            case ParticleAction::DEATH: {
                Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
//                _death_gen->add_particles(world_pos, obj->getPhysicsComponent()->getElementType());
                break;
            }
            default:
                break;
        }
    }
    
}

/*

void ParticleController::handleCharacterSpawn(GameObject* obj) {
    if (!obj->getIsPlayer()) return;
    _trail_gen->add_character(obj);
}

void ParticleController::handleCharacterDeath(GameObject* obj) {
    if (!obj->getIsPlayer()) return;
    _trail_gen->remove_character(obj);
}


// there is bug here where we get like 4 death events from collision controller per 1 monster death
void ParticleController::handleDeathParticle(GameObject* obj) {
    if (obj->type == GameObject::ObjectType::ZONE) return;
    
    Vec2 world_pos = obj->getPosition()*Util::getGamePhysicsScale();
    _death_gen->add_particles(world_pos, obj->getPhysicsComponent()->getElementType());
}

void ParticleController::handleZoneSpawn(GameObject* obj) {
    _zone_gen->add_mapping(obj);
}
*/

void ParticleController::update(float timestep, std::shared_ptr<GameState> state) {
    _trail_gen->generate();
//    _death_gen->generate();
    _pulse_gen->generate();
//    _zone_gen->generate();
}

bool ParticleController::init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
    // everything here basically needs to be replaced with JSON loading
    // i'm hardcoding the particle details in here for now and putting it into the particle map
    
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
    pd.start_alpha = 0.7;
    pd.scale = true;
    pd.current_scale = 1;
    pd.start_scale = 1;
    pd.end_scale = 0.1;
    pd.texture_name = "blue_particle";
    pd.texture = assets->get<Texture>(pd.texture_name);
    
    ParticleData pd2 = ParticleData();
    pd2.move = true;
    pd2.acceleration = -0.005;
    pd2.ttl = 40;
    pd2.color_fade = true;
    pd2.start_color = Color4f::WHITE;
    pd2.end_color = Color4f::WHITE;
    pd2.color_duration = -1;
    pd2.alpha_fade = true;
    pd2.alpha_duration = 40;
    pd2.start_alpha = 0.7;
    pd2.scale = true;
    pd2.current_scale = 1;
    pd2.start_scale = 1;
    pd2.end_scale = 0.1;
    pd2.texture_name = "gold_particle";
    pd2.texture = assets->get<Texture>(pd2.texture_name);
    
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
    temp.texture_name = "blue_death_part";
    temp.texture = assets->get<Texture>(temp.texture_name);
    
    ParticleData temp2;
    temp2.color_fade = true;
    temp2.start_color = Color4f::WHITE;
    temp2.end_color = Color4f::WHITE;
    temp2.color_duration = -1; // infinite
    temp2.move = true;
    temp2.gravity = Vec2(0,0);
    temp2.acceleration = -0.005;
    temp2.alpha_fade = true;
    temp2.alpha_duration = 50;
    temp2.start_alpha = 1;
    temp2.rotate = true;
    temp2.current_angle = 0;
    temp2.revolution_time = 2;
    temp2.scale = true;
    temp2.start_scale = 0.3;
    temp2.end_scale = 0.1;
    temp2.current_scale = 0.5;
    temp2.texture_name = "green_death_part";
    temp2.texture = assets->get<Texture>(temp2.texture_name);
    
    
    // circle of the zone
    ParticleData circlepd_temp;
    circlepd_temp.color_fade = true;
    circlepd_temp.start_color = Color4f::RED;
    circlepd_temp.end_color = Color4f::RED;
    circlepd_temp.color_duration = -1; // infinite
    circlepd_temp.scale = true;
    circlepd_temp.current_scale = 1.25;
    circlepd_temp.start_scale = 1.25;
    circlepd_temp.end_scale = 1.25;
    circlepd_temp.texture_name = "t_portalbackground";
    circlepd_temp.texture = assets->get<Texture>(circlepd_temp.texture_name);
    
    
    // ring around the zone
    ParticleData ringpd_temp;
    ringpd_temp.color_fade = true;
    ringpd_temp.start_color = Color4f::RED;
    ringpd_temp.end_color = Color4f::RED;
    ringpd_temp.color_duration = 1; // infinite
    ringpd_temp.scale = true;
    ringpd_temp.current_scale = 1.25;
    ringpd_temp.start_scale = 1.25;
    ringpd_temp.end_scale = 1.25;
    ringpd_temp.rotate = true;
    ringpd_temp.texture_name = "t_portalcircle";
    ringpd_temp.texture = assets->get<Texture>(ringpd_temp.texture_name);
    
    // pulse for spawning
    ParticleData pulsepd;
    pulsepd.ttl = 180;
    pulsepd.color_fade = true;
    pulsepd.start_color = Color4f::WHITE;
    pulsepd.end_color = Color4f::WHITE;
    pulsepd.color_duration = -1; // infinite
    pulsepd.scale = true;
    pulsepd.start_scale = 0.05;
    pulsepd.end_scale = 1;
    pulsepd.current_scale = 0.05;
    pulsepd.alpha_fade = true;
    pulsepd.start_alpha = 0.5;
    pulsepd.alpha_duration = 120;
    pulsepd.texture_name = "pulse";
    pulsepd.texture = assets->get<Texture>(pulsepd.texture_name);
    
    _particle_map.insert(std::make_pair("blue_trail", pd)); // blue trail
    _particle_map.insert(std::make_pair("gold_trail", pd2)); // gold trail
    _particle_map.insert(std::make_pair("blue_death", temp)); // blue death
    _particle_map.insert(std::make_pair("gold_death", temp2)); // gold death
    _particle_map.insert(std::make_pair("zone_circle", circlepd_temp)); // circle
    _particle_map.insert(std::make_pair("zone_ring", ringpd_temp)); // ring
    _particle_map.insert(std::make_pair("pulse_ring", pulsepd)); // ring
    
    _trail_gen = TrailParticleGenerator::alloc(state, &_particle_map);
    
//    _death_gen = DeathParticleGenerator::alloc(state, &_particle_map);
//    _death_gen->start();
    
    _pulse_gen = PulseParticleGenerator::alloc(state, &_particle_map);
    _pulse_gen->start();
    
//    _zone_gen = ZoneParticleGenerator::alloc(state, &_particle_map);
//    _zone_gen->start();
    
    return true;
}

