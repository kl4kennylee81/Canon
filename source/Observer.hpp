//
//  Observer.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Observer_hpp
#define Observer_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"

/**
 * Pure Virtual Abstract Class
 */
class Observer {
public:
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void update(Event* e);
};

#endif /* Observer_hpp */
