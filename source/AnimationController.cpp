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

#define ANIMATION_SCALE_BUFFER     1.3 // a constant needed to make the animation a bit bigger than the bounding box.
#define BLUE_COLOR   Color4::BLUE
#define RED_COLOR   Color4::RED
#define DEBUG_COLOR  Color4::GREEN
#define RED_COLOR Color4::RED

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
                    handleAction(obj, AnimationAction::DEATH);
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
                    handleAction(obj, AnimationAction::SPAWNING);
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    handleAction(obj, AnimationAction::SPAWN);
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
                    handleAction(obj, AnimationAction::ATTACK);
                    break;
            }
            break;
        }
        case Event::EventType::MOVE: {
            MoveEvent* moveEvent = (MoveEvent*)e;
            GameObject* obj = moveEvent->_character.get();
            handleAction(obj, AnimationAction::RETURN);
            break;
        }
        case Event::EventType::SWITCH: {
            SwitchEvent* switchEvent = (SwitchEvent*)e;
            GameObject* obj = switchEvent->_character.get();
            handleAction(obj, AnimationAction::ACTIVE);
            break;
        }
        case Event::EventType::ZONE: {
            ZoneEvent* zoneEvent = (ZoneEvent*)e;
            switch (zoneEvent->zoneEventType) {
                case ZoneEvent::ZoneEventType::ZONE_INIT: {
                    ZoneInitEvent* zoneInit = (ZoneInitEvent*)zoneEvent;
                    addAnimation(zoneInit->object.get(), zoneInit->animationData);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_SPAWN: {
                    ZoneSpawnEvent* zoneSpawn = (ZoneSpawnEvent*)zoneEvent;
                    handleAction(zoneSpawn->object, AnimationAction::SPAWN);
                    std::shared_ptr<AnimationNode> anim = animationMap.at(zoneSpawn->object)->getAnimationNode();
                    
                    // TODO change the colors to a macro
                    if (zoneSpawn->object->getPhysicsComponent()->getElementType() == Element::BLUE) {
						anim->setColor(Color4f(0x0EAAEB)*Color4f(1,1,1,0.5));
                    } else {
                        anim->setColor(Color4f(Color4::RED)*Color4f(1,1,1,0.5));
                    }
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_ON: {
                    ZoneOnEvent* zoneOn = (ZoneOnEvent*)zoneEvent;
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_OFF: {
                    ZoneOffEvent* zoneOff = (ZoneOffEvent*)zoneEvent;
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_DELETE: {
                    ZoneDeleteEvent* zoneDelete = (ZoneDeleteEvent*)zoneEvent;
                    animationMap.at(zoneDelete->object)->setLastAnimation();
                    break;
                }
            }
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
    animationMap.insert({obj, anim});
}

/**
 * Defers handling to the active animation handleEvent()
 */
void AnimationController::handleAction(GameObject* obj, AnimationAction action) {
    std::shared_ptr<ActiveAnimation> anim = animationMap.at(obj);
    anim->handleAction(action);
    if (anim->getAnimationNode()->getParent() == nullptr){
        syncAnimation(anim,obj);
        int zaxis = 1;
        if (obj->type == GameObject::ObjectType::ZONE){
            zaxis = 0;
        }
        if (obj->type == GameObject::ObjectType::BULLET){
            zaxis = 2;
        }
        _worldnode->addChild(anim->getAnimationNode(),zaxis);
        _worldnode->sortZOrder();
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
            syncAnimation(anim,obj);
        }
    }
}

void AnimationController::syncAnimation(std::shared_ptr<ActiveAnimation> activeAnim, GameObject* obj){
    std::shared_ptr<AnimationNode> anim = activeAnim->getAnimationNode();
    anim->setPosition(obj->getPosition());
    
    anim->setAngle(obj->getPhysicsComponent()->getBody()->getAngle());
    
    // TODO: switch from boxObstacle to PolygonObstacle
    // sync polygon obstacle size to the texture size.
    std::shared_ptr<PolygonObstacle> polyOb = std::dynamic_pointer_cast<PolygonObstacle>(obj->getPhysicsComponent()->getBody());
    Size polySize;
    if (polyOb != nullptr){
        polySize = polyOb->getSize();
    }
    if (activeAnim->isUniformScaling()){
        Size animationSize = anim->getContentSize();
        
        // maximum of boxSize.width/animation.width and boxsize.height/animation.height
        // so that the animationNode is always encapsulating the full physics box with padding.
        // this is for if we wanted to increase the size of the character, you'd only have to increase the physics box size
        
        float scaleX = (polySize.width)/animationSize.width;
        float scaleY = (polySize.height)/animationSize.height;
        float animationScale = std::max(scaleX,scaleY) * ANIMATION_SCALE_BUFFER; // to make the animation bigger than the bounding box
        anim->setScale(animationScale);
    } else {
        Size animationSize = anim->getContentSize();
        
        // maximum of boxSize.width/animation.width and boxsize.height/animation.height
        // so that the animationNode is always encapsulating the full physics box with padding.
        // this is for if we wanted to increase the size of the character, you'd only have to increase the physics box size
        
        float scaleX = (polySize.width)/animationSize.width;
        float scaleY = (polySize.height)/animationSize.height;
        Vec2 animationScale = Vec2::Vec2(scaleX,scaleY);
        anim->setScale(animationScale);
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
    
    // TODO change to load from event
//    for(auto it = playerObjects.begin() ; it != playerObjects.end(); ++it) {
//        if (it->get()->getUid()==0){
//            std::shared_ptr<AnimationData> charGirl = assets->get<AnimationData>("blueCharAnimation");
//            
//            addAnimation(it->get(), charGirl);
//            handleAction(it->get(), AnimationAction::SPAWN);
//        } else {
//            std::shared_ptr<AnimationData> charBoy = assets->get<AnimationData>("redCharAnimation");
//            
//            addAnimation(it->get(), charBoy);
//            handleAction(it->get(), AnimationAction::SPAWN);
//        }
//    }
    return true;
}
