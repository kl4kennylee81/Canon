//
//  StaticZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef StaticZoneData_hpp
#define StaticZoneData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "ZoneData.hpp"
#include "GameState.hpp"

class StaticZoneData : public ZoneData {
public:
    
    std::string objectKey;
    cugl::Vec2 relPos;
    int cooldown;
    int duration;
    ElementDataType elementType;
    
    StaticZoneData() : ZoneData(){}
    
    bool init(std::string objectKey, float relX, float relY, int cooldown, int duration, ElementDataType element) {
        ZoneData::init(ZoneType::STATIC);
        this->objectKey = objectKey;
        this->relPos.x = relX;
        this->relPos.y = relY;
        this->cooldown = cooldown;
        this->duration = duration;
        this->elementType = element;
        return true;
    }
    
    static std::shared_ptr<StaticZoneData> alloc(std::string objectKey, float relX, float relY, int cooldown, int duration, ElementDataType element) {
        std::shared_ptr<StaticZoneData> result = std::make_shared<StaticZoneData>();
        return (result->init(objectKey,relX,relY,cooldown,duration,element) ? result : nullptr);
    }
    
	virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    cugl::Vec2 getPosition(cugl::Vec2 objPos);
};

#endif /* StaticZoneData_hpp */
