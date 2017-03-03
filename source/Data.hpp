//
//  Data.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class Data : cugl::Asset {
protected:
    int _uid;
public:
    Data() :
    Asset(),
    _uid(0){}
    
    bool init(){
        return true;
    }
    
    // setting equal to 0 is a pure virtual function equivalent to an abstract function
    virtual std::string serialize() = 0;
        
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();;
};

#endif /* Data_hpp */
