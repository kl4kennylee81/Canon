//
//  ClockController.cpp
//  Canon
//
//  Created by Hong Jeon on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ClockController.hpp"
#include "PathEvent.hpp"
#include "GameState.hpp"

using namespace cugl;

/**
 * The factor that the game slows down by
 */
#define SLOW_TIME_DILATION 0.2f

ClockController::ClockController():
BaseController(){}

void ClockController::dispose(){

}

void ClockController::attach(Observer* obs) {
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
        case Event::EventType::PATH:
        {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
                case PathEvent::PathEventType::DRAWING:
                {
                    // update the clock to slow time dilation
                    GameState::_internalClock->setTimeDilation(SLOW_TIME_DILATION);
                    break;
                }
                case PathEvent::PathEventType::PATH_FINISHED:
                {
                    // Player is done drawing
                    GameState::_internalClock->setTimeDilation(1);
                    break;
                }
            }
            break;
        }
    }
}

/**
 * Increments the global frame clock for everyone to use
 */
void ClockController::update(float timestep) {
    GameState::_internalClock->incrementTime(GameState::_internalClock->getTimeDilation());
}

bool ClockController::init() {
    return true;
}

