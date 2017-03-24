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

class ObjectData : public Data {
public:
    std::string shapeKey;
    std::string animationKey;
    
    ObjectData() : Data(){}
    
    bool init(std::string shape_key,std::string anim_key);
    
    static std::shared_ptr<ObjectData> alloc(std::string shapeKey,std::string animKey){
        std::shared_ptr<ObjectData> result = std::make_shared<ObjectData>();
        return (result->init(shapeKey,animKey) ? result : nullptr);
    }

    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
};

#endif /* ObjectData_hpp */
