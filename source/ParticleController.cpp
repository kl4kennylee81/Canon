//
//  ParticleController.cpp
//  Canon
//
//  Created by Hong Jeon on 4/10/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ParticleController.hpp"
#include "PathEvent.hpp"

ParticleController::ParticleController(): BaseController(){}


void ParticleController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void ParticleController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void ParticleController::notify(Event* e) {
    BaseController::notify(e);
}
void ParticleController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::PATH:
        {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
//                case PathEvent::PathEventType::DRAWING:
//                {
//                    // update the clock to slow time dilation
//                    GameState::_internalClock->setTimeDilation(SLOW_TIME_DILATION);
//                    break;
//                }
//                case PathEvent::PathEventType::PATH_FINISHED:
//                {
//                    // Player is done drawing
//                    GameState::_internalClock->setTimeDilation(1);
//                    break;
//                }
                // path event for input?
            }
            break;
        }
    }
}


void ParticleController::update(float timestep) {
}

bool ParticleController::init() {
    return true;
}


