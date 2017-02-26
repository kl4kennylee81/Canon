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
    virtual void attach (std::shared_ptr<Observer> obs);
    
    virtual void detach (Observer* obs);
    
    virtual void notify (Event* e);
};

#endif /* Subject_hpp */
