//
//  MenuController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuController.hpp"

using namespace cugl;

MenuController::MenuController() :
BaseController(),
_menuGraph(nullptr){}

void MenuController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void MenuController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void MenuController::notify(Event* e) {
    BaseController::notify(e);
}

void MenuController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        default:
            break;
    }
}

void MenuController::update(float timestep) {
}

bool MenuController::init() {
    _menuGraph = MenuGraph::alloc();
    return true;
}

Mode MenuController::getMode(){
    return _menuGraph->getMode();
}

void MenuController::setMode(Mode m){
    _menuGraph->setMode(m);
}
