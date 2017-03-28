//
//  ClockController.cpp
//  Canon
//
//  Created by Hong Jeon on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ClockController.hpp"

using namespace cugl;

ClockController::ClockController():
BaseController(){}


void ClockController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void ClockController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void ClockController::notify(Event* e) {
    BaseController::notify(e);
}
void ClockController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::LEVEL: {
            break;
        }
    }
}

void ClockController::update(float timestep) {

}

bool ClockController::init() {
    // do initialization here
    
    return true;
}

