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
#include "CollisionEvent.hpp"
#include "Element.hpp"
#include <math.h>

using namespace cugl;

#define STATIC_BLINK_FRAMES 50

ZoneController::ZoneController():
BaseController(){}

void ZoneController::attach(Observer* obs) {
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
                    handleObjectSpawn(obj);
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
                    removeObjectZones(objectGone->_obj);
                    break;
            }
            break;
        }
    }
}

void ZoneController::update(float timestep) {
    for (auto obj : objsScheduledForRemoval) {
        std::shared_ptr<ZoneDeleteEvent> deleteEvent = ZoneDeleteEvent::alloc(obj);
        notify(deleteEvent.get());
    }
    objsScheduledForRemoval.clear();
    
    for (auto it = zoneMap.begin(); it != zoneMap.end(); it++) {
        if (!it->second->spawned){
            continue;
        }
        for (auto zd : it->second->datas) {
            switch (zd.first->type) {
                case ZoneType::STATIC: {
                    std::shared_ptr<StaticZoneData> data = std::static_pointer_cast<StaticZoneData>(zd.first);
                    updateStaticZone(it->first, it->second, data, zd.second);
                    break;
                }
                case ZoneType::ROTATE: {
                    std::shared_ptr<RotateZoneData> data = std::static_pointer_cast<RotateZoneData>(zd.first);
                    updateRotateZone(it->first, it->second, data, zd.second);
                    break;
                }
                case ZoneType::PULSE: {
                    std::shared_ptr<PulseZoneData> data = std::static_pointer_cast<PulseZoneData>(zd.first);
                    updatePulseZone(it->first, it->second, data, zd.second);
                    break;
                }
            }
        }
    }
}

void ZoneController::dispose(){
    _world = nullptr;
    state = nullptr;
}

void ZoneController::updateStaticZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<StaticZoneData> data, std::vector<GameObject*> zoneObjs) {

    for (auto zobj : zoneObjs) {
        //change to same speed when animations are done
        zobj->getPhysicsComponent()->getBody()->setLinearVelocity(charObj->getPhysicsComponent()->getBody()->getLinearVelocity());
    }
    
    zone->curIndex += GameState::_internalClock->getTimeDilation();
    if (zone->isOn && zone->curIndex >= data->duration) {
        zone->isOn = false;
        zone->curIndex = 0;
        for (auto zobj : zoneObjs) {
            std::shared_ptr<ZoneOffEvent> offEvent = ZoneOffEvent::alloc(zobj);
            notify(offEvent.get());
        }
    }
    if (!zone->isOn){
        //if we should tell animationcontroller to blink this zone
        if (data->cooldown-zone->curIndex <= STATIC_BLINK_FRAMES && !zone->blinking) {
            for (auto zobj : zoneObjs) {
                std::shared_ptr<ZoneFlashEvent> flashEvent = ZoneFlashEvent::alloc(zobj);
                notify(flashEvent.get());
            }
            zone->blinking = true;
        }
        if (zone->curIndex >= data->cooldown) {
            zone->isOn = true;
            zone->curIndex = 0;
            zone->blinking = false;
            for (auto zobj : zoneObjs) {
                std::shared_ptr<ZoneOnEvent> onEvent = ZoneOnEvent::alloc(zobj);
                notify(onEvent.get());
            }
        }
    }
}

void ZoneController::updateRotateZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<RotateZoneData> data, std::vector<GameObject*> zoneObjs) {
    
    for (auto zobj : zoneObjs) {
        //change to same speed when animations are done
        auto obst = zobj->getPhysicsComponent()->getBody();
        float curAngle = obst->getAngle();
        
        obst->setLinearVelocity(charObj->getPhysicsComponent()->getBody()->getLinearVelocity()+Vec2::forAngle(curAngle)*data->speed*GameState::_internalClock->getTimeDilation());
        
        cugl::Vec2 relPos = zobj->getPosition()-charObj->getPosition();
        obst->setAngle(relPos.getAngle()-M_PI/2);
        
    }
}

void ZoneController::updatePulseZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<PulseZoneData> data, std::vector<GameObject*> zoneObjs) {
    
    for (auto zobj : zoneObjs) {
        //change to same speed when animations are done
        zobj->getPhysicsComponent()->getBody()->setLinearVelocity(charObj->getPhysicsComponent()->getBody()->getLinearVelocity());
    }
    
    //growing bigger/max size
    if (zone->isOn) {
        //max size
        if (zone->sizeScale >= data->maxSize) {
            zone->curIndex += GameState::_internalClock->getTimeDilation();
            if (zone->curIndex >= data->maxTime) {
                zone->isOn = false;
                zone->curIndex = 0;
            }
        //growing
        } else {
            zone->sizeScale += data->speed*GameState::_internalClock->getTimeDilation();
            for (auto zobj : zoneObjs) {
                //change to same speed when animations are done
                zobj->getPhysicsComponent()->getBody()->setSize(zone->origSize*zone->sizeScale);
            }
        }
    } else {
        if (zone->sizeScale <= data->minSize) {
            zone->curIndex += GameState::_internalClock->getTimeDilation();;
            if (zone->curIndex >= data->minTime) {
                zone->isOn = true;
                zone->curIndex = 0;
            }
        } else {
            zone->sizeScale -= data->speed*GameState::_internalClock->getTimeDilation();
            for (auto zobj : zoneObjs) {
                //change to same speed when animations are done
                zobj->getPhysicsComponent()->getBody()->setSize(zone->origSize*zone->sizeScale);
            }
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
                    staticZoneInit(activeZone, data, obj);
                    break;
                }
                case ZoneType::ROTATE: {
                    std::shared_ptr<RotateZoneData> data = std::static_pointer_cast<RotateZoneData>(zd);
                    rotateZoneInit(activeZone, data, obj);
                    break;
                }
                case ZoneType::PULSE: {
                    std::shared_ptr<PulseZoneData> data = std::static_pointer_cast<PulseZoneData>(zd);
                    pulseZoneInit(activeZone, data, obj);
                    break;
                }
            }
        }
    }
}

void ZoneController::staticZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<StaticZoneData> data, GameObject* gameObj) {
    cugl::Vec2 objPos = gameObj->getPosition();
    ElementType zoneElement = Element::elementDataTypeToElement(data->elementType ,gameObj->getPhysicsComponent()->getElementType());
    std::shared_ptr<ObjectData> od = _world->getObjectData(data->objectKey);
    std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
    std::shared_ptr<AnimationData> ad = _world->getAnimationData(od->getAnimationKey(zoneElement));
    std::shared_ptr<GameObject> zone = GameObject::alloc();
    zone->type = GameObject::ObjectType::ZONE;
    
    std::shared_ptr<ZoneInitEvent> initevent = ZoneInitEvent::alloc(zone,ad,sd,objPos+data->relPos,zoneElement);
    notify(initevent.get());
    
    state->addEnemyGameObject(zone);
    
    std::vector<GameObject*> objs = {zone.get()};
    activeZone->datas.push_back(std::make_pair(data,objs));
    activeZone->isOn = false;
    activeZone->curIndex = 0;
}

void ZoneController::rotateZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<RotateZoneData> data, GameObject* gameObj) {
    cugl::Vec2 objPos = gameObj->getPosition();
    std::vector<GameObject*> objs = {};
    for (auto zEntry : data->zones) {
        ElementType zoneElement = Element::elementDataTypeToElement(zEntry->elementType ,gameObj->getPhysicsComponent()->getElementType());
        
        std::shared_ptr<ObjectData> od = _world->getObjectData(zEntry->objectKey);
        std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
        std::shared_ptr<AnimationData> ad = _world->getAnimationData(od->getAnimationKey(zoneElement));
        std::shared_ptr<GameObject> zone = GameObject::alloc();
        zone->type = GameObject::ObjectType::ZONE;
        
        float angle = zEntry->startingPosition*2*M_PI;
        cugl::Vec2 offset;
        offset.x = - (sin(angle));
        offset.y = cos(angle);
        offset *= data->radius;
    
        std::shared_ptr<ZoneInitEvent> initevent = ZoneInitEvent::alloc(zone,ad,sd,objPos+offset,zoneElement);
        notify(initevent.get());
        
        state->addEnemyGameObject(zone);
        objs.push_back(zone.get());
        
        zone->getPhysicsComponent()->getBody()->setAngle(angle);
    }
    activeZone->datas.push_back(std::make_pair(data,objs));
    activeZone->isOn = true;
    activeZone->curIndex = 0;
    
    
}

void ZoneController::pulseZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<PulseZoneData> data, GameObject* gameObj) {
    cugl::Vec2 objPos = gameObj->getPosition();
    ElementType zoneElement = Element::elementDataTypeToElement(data->elementType ,gameObj->getPhysicsComponent()->getElementType());
    
    std::shared_ptr<ObjectData> od = _world->getObjectData(data->objectKey);
    std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
    std::shared_ptr<AnimationData> ad = _world->getAnimationData(od->getAnimationKey(zoneElement));
    std::shared_ptr<GameObject> zone = GameObject::alloc();
    zone->type = GameObject::ObjectType::ZONE;
    
    std::shared_ptr<ZoneInitEvent> initevent = ZoneInitEvent::alloc(zone,ad,sd,objPos,zoneElement);
    notify(initevent.get());
    
    state->addEnemyGameObject(zone);
    
    std::vector<GameObject*> objs = {zone.get()};
    activeZone->datas.push_back(std::make_pair(data,objs));
    activeZone->isOn = true;
    activeZone->curIndex = 0;
    activeZone->origSize = zone->getPhysicsComponent()->getBody()->getSize();
    activeZone->sizeScale = data->minSize;
}

void ZoneController::handleObjectSpawn(GameObject* obj) {
    if (zoneMap.find(obj) == zoneMap.end()) {
        return;
    }
    
    std::shared_ptr<ActiveZone> activeZone = zoneMap.at(obj);
    activeZone->spawned = true;
    
    //temp
    for (auto it : activeZone->datas){
        for (auto obj : it.second) {
            std::shared_ptr<ZoneSpawnEvent> spawnEvent = ZoneSpawnEvent::alloc(obj);
            notify(spawnEvent.get());
        }
    }
}

void ZoneController::removeObjectZones(GameObject* obj) {
    if (zoneMap.find(obj) == zoneMap.end()) {
        return;
    }
    std::shared_ptr<ActiveZone> activeZone = zoneMap.at(obj);
    for (auto it : activeZone->datas) {
        for (auto zobj : it.second) {
            objsScheduledForRemoval.push_back(zobj);
        }
    }
    zoneMap.erase(obj);
    
}


