//
//  ZoneEvent.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/22/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ZoneEvent_hpp
#define ZoneEvent_hpp

#include <stdio.h>
#include "Event.hpp"
#include "GameObject.hpp"
#include "AnimationData.hpp"
#include "ShapeData.hpp"

class ZoneEvent : public Event {
public:
    enum class ZoneEventType : int {
        ZONE_INIT,
        ZONE_SPAWNING,
        ZONE_SPAWN,
        ZONE_ON,
        ZONE_OFF,
        ZONE_DELETE,
        ZONE_FLASH
    };
    
    ZoneEventType zoneEventType;
    
    ZoneEvent() : Event() {
        this->_eventType = EventType::ZONE;
    };
};

class ZoneInitEvent : public ZoneEvent {
public:
    
    std::shared_ptr<GameObject> object;
    std::shared_ptr<AnimationData> animationData;
    std::shared_ptr<ShapeData> shapeData;
    cugl::Vec2 pos;
    ElementType element;
    
    ZoneInitEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_INIT;
    }
    
    bool init(std::shared_ptr<GameObject> object, std::shared_ptr<AnimationData> animationData, std::shared_ptr<ShapeData> shapeData, cugl::Vec2 pos, ElementType element){
        this->zoneEventType = ZoneEventType::ZONE_INIT;
        this->object = object;
        this->animationData = animationData;
        this->shapeData = shapeData;
        this->pos = pos;
        this->element = element;
        return true;
    }
    
    static std::shared_ptr<ZoneInitEvent> alloc(std::shared_ptr<GameObject> object, std::shared_ptr<AnimationData> animationData, std::shared_ptr<ShapeData> shapeData, cugl::Vec2 pos, ElementType element){
        std::shared_ptr<ZoneInitEvent> result = std::make_shared<ZoneInitEvent>();
        return (result->init(object,animationData,shapeData,pos,element) ? result : nullptr);
    }
};

class ZoneSpawningEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneSpawningEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_SPAWNING;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_SPAWNING;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneSpawningEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneSpawningEvent> result = std::make_shared<ZoneSpawningEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ZoneSpawnEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneSpawnEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_SPAWN;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_SPAWN;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneSpawnEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneSpawnEvent> result = std::make_shared<ZoneSpawnEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ZoneOnEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneOnEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_ON;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_ON;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneOnEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneOnEvent> result = std::make_shared<ZoneOnEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ZoneOffEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneOffEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_OFF;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_OFF;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneOffEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneOffEvent> result = std::make_shared<ZoneOffEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ZoneDeleteEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneDeleteEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_DELETE;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_DELETE;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneDeleteEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneDeleteEvent> result = std::make_shared<ZoneDeleteEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ZoneFlashEvent : public ZoneEvent {
public:
    
    GameObject* object;
    
    ZoneFlashEvent() : ZoneEvent() {
        zoneEventType = ZoneEventType::ZONE_FLASH;
    }
    
    bool init(GameObject* object){
        this->zoneEventType = ZoneEventType::ZONE_FLASH;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ZoneFlashEvent> alloc(GameObject* object){
        std::shared_ptr<ZoneFlashEvent> result = std::make_shared<ZoneFlashEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

#endif /* ZoneEvent_hpp */
