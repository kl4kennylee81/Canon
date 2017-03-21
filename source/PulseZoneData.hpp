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
    
    PulseZoneData() : ZoneData(){}
    
    bool init();
    
    static std::shared_ptr<PulseZoneData> alloc() {
        std::shared_ptr<PulseZoneData> result = std::make_shared<PulseZoneData>();
        return (result->init() ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* PulseZoneData_hpp */
