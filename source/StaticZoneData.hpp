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
    
    StaticZoneData() : ZoneData(){}
    
    bool init();
    
    static std::shared_ptr<StaticZoneData> alloc() {
        std::shared_ptr<StaticZoneData> result = std::make_shared<StaticZoneData>();
        return (result->init() ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* StaticZoneData_hpp */
