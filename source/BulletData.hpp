//
//  BulletData.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletData_hpp
#define BulletData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "Element.hpp"

class BulletData : public Data {
private:
    std::string _objectKey;
    float _startingAngle;
    float _angleVariance;
    int _numberShots;
    int _cooldown;
    float _velocity;
    bool _target;
    ElementDataType _element;
    
public:
    
    BulletData() : Data(){}
    
    std::string getObjectKey() { return _objectKey; }
    float getStartingAngle() { return _startingAngle; }
    float getAngleVariance() { return _angleVariance; }
    int getNumberShots() { return _numberShots; }
    int getCooldown() { return _cooldown; }
    float getVelocity() { return _velocity; }
    bool isTarget() { return _target; }
    ElementDataType getElementDataType() { return _element; }
    
    bool init(std::string object, float start, float var, int num, int cool, float vel, bool tar, ElementDataType match);
    
    static std::shared_ptr<BulletData> alloc(std::string object, float start, float var, int num, int cool, float vel, bool tar, ElementDataType match)
    {
        std::shared_ptr<BulletData> result = std::make_shared<BulletData>();
        return (result->init(object, start, var, num, cool, vel, tar, match) ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    
};

#endif /* BulletData_hpp */
