//
//  ZoneController.cpp
//  Canon
//
//  Created by Jonathan Chen on 3/22/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ZoneController.hpp"
#include "ZoneEvent.hpp"
#include "LevelEvent.hpp"

using namespace cugl;

ZoneController::ZoneController():
BaseController(){}

void ZoneController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void ZoneController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void ZoneController::notify(Event* e) {
    BaseController::notify(e);
}
void ZoneController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT: {
                    ObjectInitEvent* objectInit = (ObjectInitEvent*)levelEvent;
                    addToMap(objectInit->object.get(), objectInit->zoneDatas);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    auto it = zoneMap.find(obj);
                    if (it != zoneMap.end()){
                        handleObjectSpawn(obj);
                    }
                    break;
                }
            }
            break;
        }
    }
}

void ZoneController::update(float timestep) {
    for (auto it = zoneMap.begin(); it != zoneMap.end(); it++) {
        if (!it->second->spawned){
            continue;
        }
    }
}

void ZoneController::addToMap(GameObject* obj, std::vector<std::shared_ptr<ZoneData>> datas) {
    if (!datas.empty()){
        std::shared_ptr<ActiveZone> activeZone = ActiveZone::alloc();
        zoneMap.insert({obj,activeZone});
        for (auto zd : datas) {
            switch (zd->type) {
                case ZoneType::STATIC: {
                    std::shared_ptr<StaticZoneData> data = std::static_pointer_cast<StaticZoneData>(zd);
                    staticZoneInit(activeZone, data, obj->getPosition());
                    break;
                }
            }
        }
    }
}

void ZoneController::staticZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<StaticZoneData> data, cugl::Vec2 objPos) {
    std::shared_ptr<ShapeData> sd = _world->getShapeData(data->shape_id);
    std::shared_ptr<AnimationData> ad = _world->getAnimationData(data->animation_id);
    std::shared_ptr<GameObject> zone = GameObject::alloc();
    zone->type = GameObject::ObjectType::ZONE;
    
    std::shared_ptr<ZoneInitEvent> initevent = ZoneInitEvent::alloc(zone,ad,sd,objPos+data->rel_pos,data->element);
    notify(initevent.get());
    
    state->addEnemyGameObject(zone);
    
    std::vector<GameObject*> objs = {zone.get()};
    activeZone->datas.push_back(std::make_pair(data,objs));
}

void ZoneController::handleObjectSpawn(GameObject* obj) {
    std::shared_ptr<ActiveZone> activeZone = zoneMap.at(obj);
    activeZone->spawned = true;
    
    //temp
    for (auto it : activeZone->datas){
        for (auto obj : it.second) {
            std::shared_ptr<ZoneOnEvent> onEvent = ZoneOnEvent::alloc(obj);
            notify(onEvent.get());
        }
    }
}


