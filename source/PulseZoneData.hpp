//
//  PulseZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PulseZoneData_hpp
#define PulseZoneData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "ZoneData.hpp"

class PulseZoneData : public ZoneData {
public:
    
    std::string objectKey;
    float minSize;
    int minTime;
    float maxSize;
    int maxTime;
    float speed;
    ElementDataType elementType;
    
    PulseZoneData() : ZoneData(){}
    
    bool init(std::string objectKey, float minSize, int minTime, float maxSize, int maxTime, float speed, ElementDataType element) {
        ZoneData::init(ZoneType::PULSE);
        this->objectKey = objectKey;
        this->minSize = minSize;
        this->minTime = minTime;
        this->maxSize = maxSize;
        this->maxTime = maxTime;
        this->speed = speed;
        this->elementType = element;
        return true;
    }
    
    static std::shared_ptr<PulseZoneData> alloc(std::string objectKey, float minSize, int minTime, float maxSize, int maxTime, float speed, ElementDataType element) {
        std::shared_ptr<PulseZoneData> result = std::make_shared<PulseZoneData>();
        return (result->init(objectKey,minSize,minTime,maxSize,maxTime,speed,element) ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file) override;
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual bool materialize() override;
    
    cugl::Vec2 getPosition(cugl::Vec2 objPos);
};

#endif /* PulseZoneData_hpp */
