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
#include "PathEvent.hpp"
#include "MoveEvent.hpp"
#include "SwitchEvent.hpp"
#include <cugl/cugl.h>

using namespace cugl;

#define PLAYER_BOY_YELLOW          "player_boy"
#define PLAYER_GIRL_BLUE         "player_girl"

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

//spawning spawn active hit death attack return
void AnimationController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::COLLISION: {
            CollisionEvent* collisionEvent = (CollisionEvent*)e;
            switch (collisionEvent->_collisionType) {
                case CollisionEvent::CollisionEventType::OBJECT_GONE:
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    GameObject* obj = objectGone->_obj;
                    handleEvent(obj, AnimationEvent::DEATH);
                    animationMap.at(obj)->setLastAnimation();
                    break;
            }
            break;
        }
        //add event handle for spawning
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT: {
                    ObjectInitEvent* objectInit = (ObjectInitEvent*)levelEvent;
                    addAnimation(objectInit->object.get(), objectInit->animationData);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWNING: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    handleEvent(obj, AnimationEvent::SPAWNING);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    handleEvent(obj, AnimationEvent::SPAWN);
                    break;
                }
            }
            break;
        }
        case Event::EventType::PATH: {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
                case PathEvent::PathEventType::PATH_FINISHED:
                    PathFinished* pathFinished = (PathFinished*)pathEvent;
                    GameObject* obj = pathFinished->_character.get();
                    handleEvent(obj, AnimationEvent::ATTACK);
                    break;
            }
            break;
        }
        case Event::EventType::MOVE: {
            MoveEvent* moveEvent = (MoveEvent*)e;
            GameObject* obj = moveEvent->_character.get();
            handleEvent(obj, AnimationEvent::RETURN);
            break;
        }
        case Event::EventType::SWITCH: {
            SwitchEvent* switchEvent = (SwitchEvent*)e;
            GameObject* obj = switchEvent->_character.get();
            handleEvent(obj, AnimationEvent::ACTIVE);
            break;
        }
    }
}

void AnimationController::update(float timestep,std::shared_ptr<GameState> state) {
    syncAll();
    updateFrames();
}

/**
 * Adds the animation data to the active animation map.
 */
void AnimationController::addAnimation(GameObject* obj, std::shared_ptr<AnimationData> data) {
    std::shared_ptr<ActiveAnimation> anim = ActiveAnimation::alloc();
    anim->setAnimationData(data);
    anim->setScale(1.f/GameState::getPhysicsScale());
    animationMap.insert({obj, anim});
}

/**
 * Defers handling to the active animation handleEvent()
 */
void AnimationController::handleEvent(GameObject* obj, AnimationEvent event) {
    std::shared_ptr<ActiveAnimation> anim = animationMap.at(obj);
    anim->handleEvent(event);
    if (anim->getAnimationNode()->getParent() == nullptr){
        _worldnode->addChild(anim->getAnimationNode(),1);
    }
}

/**
 * Syncs the location of the animatino node to the world coordinates of
 * the corresponding location of the node's physics body.
 */
void AnimationController::syncAll() {
    for (auto it = animationMap.begin(); it != animationMap.end(); it++) {
        GameObject* obj = it->first;
        std::shared_ptr<ActiveAnimation> anim = it->second;
        if (obj->getPhysicsComponent() != nullptr) {
            anim->getAnimationNode()->setPosition(obj->getPosition());
        }
    }
}

/**
 * Removes animations that have been completed to the last frame
 */
void AnimationController::updateFrames() {
    for (auto it = animationMap.begin(); it != animationMap.end();) {
        std::shared_ptr<ActiveAnimation> anim = it->second;
        
        if (anim->getAnimationNode()->getParent() == nullptr){
            it++;
            continue;
        }
        
        if (!anim->nextFrame()){
            anim->getAnimationNode()->removeFromParent();
            animationMap.erase(it++);
        } else {
            it++;
        }
    }
}

bool AnimationController::init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
    _worldnode = state->getWorldNode();
    
    std::vector<std::shared_ptr<GameObject>> playerObjects = state->getPlayerCharacters();
    
    //super hacky. change to only be events
    for(auto it = playerObjects.begin() ; it != playerObjects.end(); ++it) {
        if (it->get()->getUid()==0){
            std::shared_ptr<AnimationData> charGirl = assets->get<AnimationData>("blueCharAnimation");
            
            addAnimation(it->get(), charGirl);
            handleEvent(it->get(), AnimationEvent::SPAWN);
        } else {
            std::shared_ptr<AnimationData> charBoy = assets->get<AnimationData>("redCharAnimation");
            
            addAnimation(it->get(), charBoy);
            handleEvent(it->get(), AnimationEvent::SPAWN);
        }
    }
    return true;
}
