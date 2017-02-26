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

class Data {
    
    virtual std::string serialize();

    virtual void fromJson(std::string str);
};

#endif /* Data_hpp */
