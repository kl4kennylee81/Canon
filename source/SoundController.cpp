//
//  SoundController.cpp
//  Canon
//
//  Created by Jonathan Chen on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SoundController.hpp"
#include "LevelEvent.hpp"
#include "CollisionEvent.hpp"
#include "PathEvent.hpp"

#define GENERAL_SOUND_DATA_NAME "general"

using namespace cugl;

SoundController::SoundController():
BaseController(){}

void SoundController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void SoundController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void SoundController::notify(Event* e) {
    BaseController::notify(e);
}
void SoundController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::COLLISION: {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE: {
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    handleObjectAction(objectGone->_obj, SoundAction::DEATH);
                    break;
                }
                case CollisionEvent::CollisionEventType::OBJECT_HIT: {
                    ObjectHitEvent* objectHit = (ObjectHitEvent*)collisionEvent;
                    if(objectHit->_obj->type == GameObject::ObjectType::CHARACTER){
                        handleObjectAction(objectHit->_obj, SoundAction::HIT);
                    }
                    break;
                }

            }
            break;
        }
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT: {
                    ObjectInitEvent* objectInit = (ObjectInitEvent*)levelEvent;
                    objectSoundMap.insert({objectInit->object.get(), objectInit->soundData});
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    handleObjectAction(objectSpawn->object.get(), SoundAction::SPAWN);
                    break;
                }
            }
            break;
        }
        case Event::EventType::PATH:
        {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
                case PathEvent::PathEventType::DRAWING:
                {
                    handleGeneralAction(SoundAction::SLOW_MOTION_START);
                    break;
                }
                case PathEvent::PathEventType::PATH_FINISHED:
                {
                    handleGeneralAction(SoundAction::SLOW_MOTION_END);
                    break;
                }
            }
            break;
        }
    }
}

void SoundController::handleGeneralAction(SoundAction action) {
    if (generalSounds->soundMap.find(action) != generalSounds->soundMap.end()) {
        std::string sound = generalSounds->soundMap.at(action);
        std::string audioKey = sound + std::to_string(step);
        if (!AudioEngine::get()->isActiveEffect(sound+audioKey)) {
            auto source = _world->getAssetManager()->get<Sound>(sound);
            AudioEngine::get()->playEffect(sound+audioKey, source, false, source->getVolume());
        }
    }
}

void SoundController::handleObjectAction(GameObject* obj, SoundAction action) {
    std::shared_ptr<SoundData> sd = objectSoundMap.at(obj);
    if(sd == nullptr) return;
    if (sd->soundMap.find(action) != sd->soundMap.end()) {
        std::string sound = sd->soundMap.at(action);
        std::string audioKey = sound + std::to_string(step);
        if (!AudioEngine::get()->isActiveEffect(sound+audioKey)) {
            auto source = _world->getAssetManager()->get<Sound>(sound);
            AudioEngine::get()->playEffect(sound+audioKey, source, false, source->getVolume());
        }
    }
}

bool SoundController::init(std::shared_ptr<World> world){
    this->_world = world;
    generalSounds = world->getSoundData(GENERAL_SOUND_DATA_NAME);
    return true;
};

void SoundController::update(float timestep,std::shared_ptr<GameState> state) {
    step++;
}
