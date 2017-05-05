//
//  BulletController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "BulletController.hpp"
#include "MoveController.hpp"
#include "Element.hpp"
#include "PathParameters.h"
#include "HomingAI.hpp"
#include "LevelEvent.hpp"
#include "CollisionEvent.hpp"
#include "PathAI.hpp"
#include "StaticAI.hpp"

using namespace cugl;

BulletController::BulletController():
BaseController(){}

void BulletController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void BulletController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void BulletController::notify(Event* e) {
    BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void BulletController::eventUpdate(Event* e){
    switch (e->_eventType) {
        case Event::EventType::LEVEL:
        {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT:
                {
                    ObjectInitEvent* init = (ObjectInitEvent*)levelEvent;
                    
                    // the player character does not have an BulletData
                    addBullet(init->bulletData->newActiveBullet(init->object));
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN:
                {
                    ObjectSpawnEvent* spawn = (ObjectSpawnEvent*)levelEvent;
                    
                    // check existence in hashmap if not init then it was likely the player character
                    if (_map.count(spawn->object.get()) <= 0) {
                        return;
                    }
                    
                    std::shared_ptr<ActiveBullet> activeBullet = _map.at(spawn->object.get());
                    activeBullet->toggleActive();
                    break;
                }
            }
            
            break;
        }
        case Event::EventType::COLLISION:
        {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                removeBullet(objectGone->_obj);
                break;
            }
            break;
        }
    }
}

void BulletController::addBullet(std::shared_ptr<ActiveBullet> bullet) {
    _enemies.insert(bullet);
    for (auto it : bullet->getObjects()) {
        _map[it.get()] = bullet;
    }
}

void BulletController::removeBullet(GameObject* obj) {
    // duplicate removeAI events are called there are cases where the AI is already removed
    if (_map.find(obj) == _map.end()) {
        return;
    }
    auto bullet = _map.at(obj);
    if (bullet->garbageCollect(obj)) {
        _enemies.erase(bullet);
    }
    _map.erase(obj);
    
}

void BulletController::update(float timestep,std::shared_ptr<GameState> state){
    for (auto it : _enemies) {
        if (it->isActive()){
            it->update(state);
        }
    }
}

bool BulletController::init() {
    return true;
}
