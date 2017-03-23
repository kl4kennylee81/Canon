//
//  ActiveZone.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/22/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActiveZone_hpp
#define ActiveZone_hpp

#include <stdio.h>
#include "GameObject.hpp"
#include "ZoneData.hpp"
#include "StaticZoneData.hpp"
#include "RotateZoneData.hpp"
#include "PulseZoneData.hpp"


class ActiveZone {
public:
    
    std::vector<std::pair<std::shared_ptr<ZoneData>,std::vector<GameObject*>>> datas;
    
    bool spawned;
    
    bool init() {
        spawned = false;
        return true;
    }
    
    static std::shared_ptr<ActiveZone> alloc() {
        std::shared_ptr<ActiveZone> result = std::make_shared<ActiveZone>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* ActiveZone_hpp */
