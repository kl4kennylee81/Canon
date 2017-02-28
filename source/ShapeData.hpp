//
//  ShapeData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ShapeData_hpp
#define ShapeData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

class ShapeData : public Data {
public:
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();

};

#endif /* ShapeData_hpp */
