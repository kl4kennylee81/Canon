//
//  RotateZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RotateZoneData_hpp
#define RotateZoneData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "ZoneData.hpp"
#include "GameState.hpp"

class ZoneEntry {
public:
    
    std::string objectKey;
    float startingPosition;
    ElementDataType elementType;
    
    bool init(std::string o, float p, ElementDataType e) {
        this->objectKey = o;
        this->startingPosition = p;
        this->elementType = e;
        return true;
    }
    
    static std::shared_ptr<ZoneEntry> alloc(std::string objectKey, float startingPosition, ElementDataType element) {
        std::shared_ptr<ZoneEntry> result = std::make_shared<ZoneEntry>();
        return (result->init(objectKey,startingPosition,element) ? result : nullptr);
    }

	std::shared_ptr<cugl::JsonValue> toJsonValue();
};

class RotateZoneData : public ZoneData {
public:
    
    std::vector<std::shared_ptr<ZoneEntry>> zones;
    float radius;
    float speed;
    
    RotateZoneData() : ZoneData(){}
    
    bool init(float radius, float speed) {
        ZoneData::init(ZoneType::ROTATE);
        this->radius = radius/GAME_PHYSICS_SCALE;
        this->speed = speed;
        return true;
    }
    
    static std::shared_ptr<RotateZoneData> alloc(float radius, float speed) {
        std::shared_ptr<RotateZoneData> result = std::make_shared<RotateZoneData>();
        return (result->init(radius,speed) ? result : nullptr);
    }
    
	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();

	virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* RotateZoneData_hpp */
