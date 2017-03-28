//
//  ClockController.cpp
//  Canon
//
//  Created by Hong Jeon on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ClockController.hpp"

using namespace cugl;

/**
 * The factor that the game slows down by
 */
#define SLOW_TIME_DILATION 0.2f

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
    return true;
}

