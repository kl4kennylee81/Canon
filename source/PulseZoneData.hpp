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
private:
    
    float _lerpTime;

public:
    
    std::string objectKey;
    
    cugl::Vec2 minSize;
    int minTime;
    
    cugl::Vec2 maxSize;
    int maxTime;
    
    ElementDataType elementType;
    
    PulseZoneData() : ZoneData(){}
    
    bool init(std::string objectKey, cugl::Vec2 minSize, int minTime, cugl::Vec2 maxSize, int maxTime, float lerpTime, ElementDataType element) {
        ZoneData::init(ZoneType::PULSE);
        this->objectKey = objectKey;
        this->minSize = cugl::Vec2::Vec2(minSize);
        this->minTime = minTime;
        this->maxSize = cugl::Vec2::Vec2(maxSize);
        this->maxTime = maxTime;
        this->elementType = element;
        this->_lerpTime = lerpTime;
        return true;
    }
    
    static std::shared_ptr<PulseZoneData> alloc(std::string objectKey, cugl::Vec2 minSize, int minTime, cugl::Vec2 maxSize, int maxTime, float lerpTime, ElementDataType element) {
        std::shared_ptr<PulseZoneData> result = std::make_shared<PulseZoneData>();
        return (result->init(objectKey,minSize,minTime,maxSize,maxTime,lerpTime,element) ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file) override;
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual bool materialize() override;
    
    cugl::Vec2 getPosition(cugl::Vec2 objPos);
    
    cugl::Vec2 getLerpSpeed();
};

#endif /* PulseZoneData_hpp */
