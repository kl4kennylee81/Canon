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

class StaticZoneData : public ZoneData {
public:
    
    int shape_id;
    int animation_id;
    cugl::Vec2 rel_pos;
    int cooldown;
    int duration;
    Element element;
    
    StaticZoneData() : ZoneData(){}
    
    bool init(int uid, int shape_id, int animation_id, float rel_x, float rel_y, int cooldown, int duration, Element element) {
        ZoneData::init(uid, ZoneType::STATIC);
        this->shape_id = shape_id;
        this->animation_id = animation_id;
        this->rel_pos.x = rel_x;
        this->rel_pos.y = rel_y;
        this->cooldown = cooldown;
        this->duration = duration;
        this->element = element;
        return true;
    }
    
    static std::shared_ptr<StaticZoneData> alloc(int uid, int shape_id, int animation_id, float rel_x, float rel_y, int cooldown, int duration, Element element) {
        std::shared_ptr<StaticZoneData> result = std::make_shared<StaticZoneData>();
        return (result->init(uid,shape_id,animation_id,rel_x,rel_y,cooldown,duration,element) ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* StaticZoneData_hpp */
