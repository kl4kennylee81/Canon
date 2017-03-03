//
//  AnimationController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationController.hpp"
#include "CollisionEvent.hpp"
#include "LevelEvent.hpp"
#include <cugl/cugl.h>

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
        case Event::EventType::COLLISION: {
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
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWN:
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    addToWorldNode(obj);
                    break;
            }
            break;
        }
    }
}

void AnimationController::update(float timestep,std::shared_ptr<GameState> state) {
    for (auto obj : objsToRemove) {
        obj->getNode()->removeFromParent();
        state->removeObject(obj);
    }
    objsToRemove.clear();
}

void AnimationController::addToWorldNode(GameObject* obj) {
    _worldnode->addChild(obj->getSharedPointerNode(),1);
}


bool AnimationController::init(std::shared_ptr<GameState> state) {
    _worldnode = state->getWorldNode();
    
    std::vector<std::shared_ptr<GameObject>> playerObjects = state->getPlayerCharacters();
    
    // iterate through player character gameObjects and create the PhysicsComponent
    // and add it to the ObstacleWorld
    for(auto it = playerObjects.begin() ; it != playerObjects.end(); ++it) {
        addToWorldNode(it->get());
    }
    return true;
}
