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
    std::string blueShapeKey;
    std::string blueAnimationKey;
    
    std::string goldShapeKey;
    std::string goldAnimationKey;
public:
    
    ObjectData() : Data(){}
    
    std::string getShapeKey(Element e);
    
    std::string getAnimationKey(Element e);
    
    bool init(std::string blueShapeKey,std::string blueAnimKey,std::string goldShapeKey, std::string goldAnimKey);
    
    static std::shared_ptr<ObjectData> alloc(std::string blueShapeKey,std::string blueAnimKey,std::string goldShapeKey, std::string goldAnimKey){
        std::shared_ptr<ObjectData> result = std::make_shared<ObjectData>();
        return (result->init(blueShapeKey,blueAnimKey,goldShapeKey,goldAnimKey) ? result : nullptr);
    }

    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
};

#endif /* ObjectData_hpp */
