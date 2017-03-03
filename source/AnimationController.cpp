//
//  AnimationController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationController.hpp"
#include "CollisionEvent.hpp"

using namespace cugl;

AnimationController::AnimationController():
BaseController(){}

void AnimationController::attach(std::shared_ptr<Observer> obs) {
    BaseController::attach(obs);
}
void AnimationController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void AnimationController::notify(Event* e) {
    BaseController::notify(e);
}
void AnimationController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::COLLISION:
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    GameObject* obj = objectGone->_obj;
                    objsToRemove.push_back(obj);
                    break;
            }
            break;
    }
}

void AnimationController::update(float timestep,std::shared_ptr<GameState> state) {
    for (auto obj : objsToRemove) {
        state->removeObject(obj);
    }
    objsToRemove.clear();
}

bool AnimationController::init(std::shared_ptr<GameState> state) {
    return true;
}
