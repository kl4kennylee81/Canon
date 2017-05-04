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

SpawnController::SpawnController():
BaseController(){}

void SpawnController::attach(Observer* obs) {
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
                    
                    spawnMap.insert({objectSpawning->object,objectSpawning->spawnTime});
                    break;
                }
            }
            break;
        }
    }
}

void SpawnController::update(float timestep,std::shared_ptr<GameState> state) {
    for (auto it = spawnMap.begin(); it != spawnMap.end();) {
        auto obj = it->first;
        auto frames = it->second;
        if (frames <= 0){
            std::shared_ptr<ObjectSpawnEvent> objectSpawn = ObjectSpawnEvent::alloc(obj);
            notify(objectSpawn.get());
            spawnMap.erase(it++);
        } else {
            it->second = frames - GameState::_internalClock->getTimeDilation();
            it++;
        }
    }
}

bool SpawnController::init() {
    return true;
}

std::shared_ptr<cugl::JsonValue> SpawnController::toJsonValue(){
    // TODO kelly
    // the spawnController needs to serialize its map of spawnMap
    // save it as gameObject uid -> float
    // IMPORTANT: spawnController json is actually used by the LevelController not by itself
    // since levelController has the mapping of gameObjectUid -> waveEntryID
    // it will call initEvent and SpawningEvent with the modified time according to what
    // it is currently at based on the serialized json value
    return JsonValue::allocObject();
}
