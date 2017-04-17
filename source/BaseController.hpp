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
#include "GameState.hpp"


class BaseController : public Observer, Subject {

protected:   
	std::vector<Observer*> _observers;
    int _uid;

    
public:
    
    BaseController();
    
    virtual ~BaseController();
    
    virtual void attach (Observer* obs);
    
    virtual void detach (Observer* obs);
    
    virtual void notify (Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e) = 0;

	int getUid() {
		return _uid++;
	}

	void deactivateButton(std::shared_ptr<cugl::Node> node, int tag);

	void deactivateButton(std::shared_ptr<cugl::Node> node, std::string name);
	bool instanceOfButton(std::shared_ptr<cugl::Node> node);
	void deactivateAllButtons(std::shared_ptr<cugl::Node> node);
	void deactivateAndClear(std::shared_ptr<cugl::Node> node);
};
#endif /* BaseController_hpp */
