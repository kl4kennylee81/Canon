//
//  BaseController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BaseController.hpp"

using namespace cugl;

void BaseController::attach(std::shared_ptr<Observer> obs) {
	_observers.push_back(obs);
}

void BaseController::detach(std::shared_ptr<Observer> obs) {
	_observers.erase(obs);
}

void BaseController::notify(Event* e) {
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		it->get()->eventUpdate(e);
	}
}