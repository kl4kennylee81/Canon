//
//  LevelEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelEvent_hpp
#define LevelEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"
#include "GameObject.hpp"
#include "ObjectData.hpp"
#include "AnimationData.hpp"
#include "ShapeData.hpp"
#include "WaveData.hpp"
#include "AIData.hpp"

class LevelEvent : public Event {
public:
    enum class LevelEventType : int {
        /** Signal that a path is done */
        OBJECT_INIT,
        OBJECT_SPAWNING,
        OBJECT_SPAWN
    };
    
    LevelEventType levelEventType;
    
    LevelEvent() : Event() {
        this->_eventType = EventType::LEVEL;
    };
};

class ObjectInitEvent : public LevelEvent {
public:
    
    std::shared_ptr<GameObject> object;
    std::shared_ptr<WaveEntry> waveEntry;
    std::shared_ptr<ObjectData> objectData;
    std::shared_ptr<AnimationData> animationData;
    std::shared_ptr<ShapeData> shapeData;
	std::shared_ptr<AIData> aiData;
    
    ObjectInitEvent() : LevelEvent() {
        levelEventType = LevelEventType::OBJECT_INIT;
    }
    
    bool init(std::shared_ptr<GameObject> object, std::shared_ptr<WaveEntry> waveEntry, std::shared_ptr<ObjectData> objectData, 
		std::shared_ptr<AnimationData> animationData, std::shared_ptr<ShapeData> shapeData, std::shared_ptr<AIData> aiData){
        this->levelEventType = LevelEventType::OBJECT_INIT;
        this->object = object;
        this->waveEntry = waveEntry;
        this->objectData = objectData;
        this->animationData = animationData;
        this->shapeData = shapeData;
		this->aiData = aiData;
        return true;
    }
    
    static std::shared_ptr<ObjectInitEvent> alloc(std::shared_ptr<GameObject> object, std::shared_ptr<WaveEntry> waveEntry, 
		std::shared_ptr<ObjectData> objectData, std::shared_ptr<AnimationData> animationData, std::shared_ptr<ShapeData> shapeData,
		std::shared_ptr<AIData> aiData)
	{
        std::shared_ptr<ObjectInitEvent> result = std::make_shared<ObjectInitEvent>();
        return (result->init(object,waveEntry,objectData,animationData,shapeData, aiData) ? result : nullptr);
    }
};

class ObjectSpawningEvent : public LevelEvent {
public:
    
    std::shared_ptr<GameObject> object;
    
    ObjectSpawningEvent() : LevelEvent() {
        levelEventType = LevelEventType::OBJECT_SPAWNING;
    }
    
    bool init(std::shared_ptr<GameObject> object){
        this->levelEventType = LevelEventType::OBJECT_SPAWNING;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ObjectSpawningEvent> alloc(std::shared_ptr<GameObject> object){
        std::shared_ptr<ObjectSpawningEvent> result = std::make_shared<ObjectSpawningEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

class ObjectSpawnEvent : public LevelEvent {
public:
    
    std::shared_ptr<GameObject> object;
    
    ObjectSpawnEvent() : LevelEvent() {
        levelEventType = LevelEventType::OBJECT_SPAWN;
    }
    
    bool init(std::shared_ptr<GameObject> object){
        this->levelEventType = LevelEventType::OBJECT_SPAWN;
        this->object = object;
        return true;
    }
    
    static std::shared_ptr<ObjectSpawnEvent> alloc(std::shared_ptr<GameObject> object){
        std::shared_ptr<ObjectSpawnEvent> result = std::make_shared<ObjectSpawnEvent>();
        return (result->init(object) ? result : nullptr);
    }
};

#endif /* LevelEvent_hpp */
