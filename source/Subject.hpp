//
//  Subject.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Subject_hpp
#define Subject_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Observer.hpp"

class Subject {
public:
    virtual void attach (Observer* obs) = 0;
    
    virtual void detach (Observer* obs) = 0;
    
    virtual void notify (Event* e) = 0;
};

#endif /* Subject_hpp */
