//
//  BaseController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BaseController.hpp"

using namespace cugl;

BaseController::BaseController(){
}

BaseController::~BaseController(){
}

void BaseController::attach(std::shared_ptr<Observer> obs) {
	_observers.push_back(obs);
}

void BaseController::detach(Observer* obs) {
    for(auto it = _observers.begin(); it != _observers.end(); ++it) {
        if (it->get() == obs) {
            _observers.erase(it);
            return;
        }
    }
}

void BaseController::notify(Event* e) {
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		it->get()->eventUpdate(e);
	}
}
