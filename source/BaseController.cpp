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

void BaseController::attach(Observer* obs) {
	_observers.push_back(obs);
}

void BaseController::detach(Observer* obs) {
    for(auto it = _observers.begin(); it != _observers.end(); ++it) {
        if ((*it) == obs) {
            _observers.erase(it);
            return;
        }
    }
}

void BaseController::notify(Event* e) {
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		(*it)->eventUpdate(e);
	}
}

void BaseController::deactivateButton(std::shared_ptr<Node> node, int tag) {
	auto buttonNode = node->getChildByTag(tag);
	if (buttonNode != nullptr) {
		auto button = std::static_pointer_cast<Button>(buttonNode);
		button->deactivate();
	}
}

void BaseController::deactivateButton(std::shared_ptr<Node> node, std::string name) {
	auto buttonNode = node->getChildByName(name);
	if (buttonNode != nullptr) {
		auto button = std::static_pointer_cast<Button>(buttonNode);
		button->deactivate();
	}
}
