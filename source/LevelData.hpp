//
//  LevelData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelData_hpp
#define LevelData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

class LevelData : public Data {
public:
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* LevelData_hpp */
