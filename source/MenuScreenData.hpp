//
//  MenuScreenData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MenuScreenData_hpp
#define MenuScreenData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

using std::string;


class MenuScreenData : Data {
public:    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* MenuScreenData_hpp */
