//
//  SpawnController.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "SpawnController.hpp"
#include "LevelEvent.hpp"

using namespace cugl;

#define SPAWN_FRAMES 30.0f

SpawnController::SpawnController():
BaseController(){}

void SpawnController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void SpawnController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void SpawnController::notify(Event* e) {
    BaseController::notify(e);
}
void SpawnController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWNING: {
                    ObjectSpawningEvent* objectSpawning = (ObjectSpawningEvent*)levelEvent;
                    spawnMap.insert({objectSpawning->object,0.0f});
                    break;
                }
            }
            break;
        }
    }
}

void SpawnController::update(float timestep) {
    for (auto it = spawnMap.begin(); it != spawnMap.end();) {
        auto obj = it->first;
        auto frames = it->second;
        if (frames >= SPAWN_FRAMES){
            std::shared_ptr<ObjectSpawnEvent> objectSpawn = ObjectSpawnEvent::alloc(obj);
            notify(objectSpawn.get());
            spawnMap.erase(it++);
        } else {
            it->second = frames + GameState::_internalClock->getTimeDilation();
            it++;
        }
    }
}

bool SpawnController::init() {
    return true;
}
