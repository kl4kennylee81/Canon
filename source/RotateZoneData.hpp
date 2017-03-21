//
//  RotateZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RotateZoneData_hpp
#define RotateZoneData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "ZoneData.hpp"

class RotateZoneData : public ZoneData {
public:
    
    RotateZoneData() : ZoneData(){}
    
    bool init();
    
    static std::shared_ptr<RotateZoneData> alloc() {
        std::shared_ptr<RotateZoneData> result = std::make_shared<RotateZoneData>();
        return (result->init() ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* RotateZoneData_hpp */
