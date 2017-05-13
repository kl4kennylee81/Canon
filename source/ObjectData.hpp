//
//  ObjectData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ObjectData_hpp
#define ObjectData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "Element.hpp"

class ObjectData : public Data {
private:
    std::string shapeKey;
    std::string soundKey;
    std::string blueAnimationKey;
    std::string goldAnimationKey;
    
    float speed;
    
    float animationScale;
    int health;
    
    std::string particleStateKey;
public:
    
    ObjectData() : Data(){}
    
    std::string getShapeKey();
    
    std::string getSoundKey();
    
    std::string getAnimationKey(ElementType e);
    
    std::string getParticleStateKey();
    
    float getSpeed();
    
    float getAnimationScale();
    
    int getHealth();
    
    bool init(std::string shapeKey,std::string blueAnimKey, std::string goldAnimKey, std::string sound_key,float animScale, int health, float speed, std::string partStateKey);
    
    static std::shared_ptr<ObjectData> alloc(std::string shapeKey,std::string blueAnimKey, std::string goldAnimKey, std::string sound_key,float animScale,int health, float speed, std::string partStateKey){
        std::shared_ptr<ObjectData> result = std::make_shared<ObjectData>();
        return (result->init(shapeKey,blueAnimKey, goldAnimKey,sound_key,animScale,health, speed, partStateKey) ? result : nullptr);
    }

    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
};

#endif /* ObjectData_hpp */
