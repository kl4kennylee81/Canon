//
//  ZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/20/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ZoneData_hpp
#define ZoneData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "Element.hpp"

enum class ZoneType : int {
    STATIC,
    ROTATE,
    PULSE
};

class ZoneData : public Data {
public:
    
    ZoneType type;
    
    /*
    int shape_id;
    int animation_id;
    Element element;
    ZoneType type;
    int speed;
    bool moveWithObject;
    cugl::Vec2 relativePosition;*/
    
    ZoneData() : Data(){}
    
    bool init(int uid, ZoneType type) {
        this->_uid = uid;
        this->type = type;
        return true;
    }
    
    static std::shared_ptr<ZoneData> alloc(int uid, ZoneType type) {
        std::shared_ptr<ZoneData> result = std::make_shared<ZoneData>();
        return (result->init(uid,type) ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};


#endif /* ZoneData_hpp */
