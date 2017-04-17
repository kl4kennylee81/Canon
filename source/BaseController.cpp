//
//  BaseController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BaseController.hpp"
#include <iostream>

using namespace cugl;

BaseController::BaseController(){
    _uid = 0;
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

void BaseController::deactivateAllButtons(std::shared_ptr<cugl::Node> node) {
	if (node == nullptr) return;
	auto button = dynamic_cast<Button*>(node.get());
	if (button != nullptr) {
		button->deactivate();
	}
	for (auto it : node->getChildren()) {
		deactivateAllButtons(it);
	}
}

void BaseController::deactivateAndClear(std::shared_ptr<cugl::Node> node) {
	if (node == nullptr) return;
	deactivateAllButtons(node);
	node->removeAllChildren();
}
