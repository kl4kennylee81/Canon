//
//  BaseController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BaseController_hpp
#define BaseController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Observer.hpp"
#include "Subject.hpp"


class BaseController : public Observer, Subject {
protected:   
	std::vector<std::shared_ptr<Observer>> _observers;
    
public:
    virtual void attach (std::shared_ptr<Observer> obs);
    
    virtual void detach (std::shared_ptr<Observer> obs);
    
    virtual void notify (Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
};
#endif /* BaseController_hpp */
