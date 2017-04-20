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
#include "AnimationEvent.hpp"
#include <cugl/cugl.h>

using namespace cugl;

#define ANIMATION_SCALE_BUFFER     1.0 // a constant needed to make the animation a bit bigger than the bounding box.
#define BLUE_COLOR   Color4::BLUE
#define GOLD_COLOR   Color4::YELLOW
#define DEBUG_COLOR  Color4::GREEN

AnimationController::AnimationController():
BaseController(){}

void AnimationController::attach(Observer* obs) {
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
                    addAnimation(objectInit->object.get(), objectInit->animationData, objectInit->objectData->getAnimationScale());
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
                    if (zoneSpawn->object->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        anim->setColor(Color4f(BLUE_COLOR)*Color4f(1,1,1,0.5));
                    } else {
                        anim->setColor(Color4f(GOLD_COLOR)*Color4f(1,1,1,0.5));
                    }
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_ON: {
                    ZoneOnEvent* zoneOn = (ZoneOnEvent*)zoneEvent;
                    GameObject* obj = zoneOn->object;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(obj)->getAnimationNode();
                    
                    // TODO change the colors to a macro
                    if (obj->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        anim->setColor(Color4f(BLUE_COLOR)*Color4f(1,1,1,0.5));
                    } else {
                        anim->setColor(Color4f(GOLD_COLOR)*Color4f(1,1,1,0.5));
                    }
                    animationMap.at(zoneOn->object)->setFlash(false);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_OFF: {
                    ZoneOffEvent* zoneOff = (ZoneOffEvent*)zoneEvent;
                    GameObject* obj = zoneOff->object;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(obj)->getAnimationNode();
                    
                    // TODO change the colors to a macro
                    if (obj->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        anim->setColor(Color4f(BLUE_COLOR)*Color4f(1,1,1,0.1));
                    } else {
                        anim->setColor(Color4f(GOLD_COLOR)*Color4f(1,1,1,0.1));
                    }
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_DELETE: {
                    ZoneDeleteEvent* zoneDelete = (ZoneDeleteEvent*)zoneEvent;
                    animationMap.at(zoneDelete->object)->setLastAnimation();
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_FLASH: {
                    ZoneFlashEvent* zoneFlash = (ZoneFlashEvent*)zoneEvent;
                    animationMap.at(zoneFlash->object)->setFlash(true);
                }
            }
            break;
        }
    }
}



void AnimationController::update(float timestep,std::shared_ptr<GameState> state) {
    syncAll();
    updateFrames(state);
}

/**
 * Adds the animation data to the active animation map.
 */
void AnimationController::addAnimation(GameObject* obj, std::shared_ptr<AnimationData> data,float animScale) {
    std::shared_ptr<ActiveAnimation> anim = ActiveAnimation::alloc(animScale);
    anim->setAnimationData(data);
    animationMap.insert({obj, anim});
}

/**
 * Defers handling to the active animation handleEvent()
 */
void AnimationController::handleAction(GameObject* obj, AnimationAction action) {
    std::shared_ptr<ActiveAnimation> anim = animationMap.at(obj);
    if (anim->isLastAnimation()){ return;}
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
        float animationScale = std::max(scaleX,scaleY) * activeAnim->getAnimationScale(); // to make the animation bigger than the bounding box
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
void AnimationController::updateFrames(std::shared_ptr<GameState> state) {
    for (auto it = animationMap.begin(); it != animationMap.end();) {
        std::shared_ptr<ActiveAnimation> anim = it->second;
        
        if (anim->getAnimationNode()->getParent() == nullptr){
            it++;
            continue;
        }
        
        if (anim->getFlash()){
            anim->flashIndex += GameState::_internalClock->getTimeDilation();
            if (anim->flashIndex >= 10) {
                anim->flashIndex = 0;
                anim->lit = !anim->lit;
            }
            
            float alphascale = anim->lit ? 0.5 : 0.1;
            
            if (it->first->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                anim->getAnimationNode()->setColor(Color4f(BLUE_COLOR)*Color4f(1,1,1,alphascale));
            } else {
                anim->getAnimationNode()->setColor(Color4f(GOLD_COLOR)*Color4f(1,1,1,alphascale));
            }
            
        }
        
        if (!anim->nextFrame()){
            removeAnimation(state,it->first,it->second);
			animationMap.erase(it);
            break;
        } else {
            it++;
        }
    }
}

void AnimationController::removeAnimation(std::shared_ptr<GameState> state, GameObject* obj,std::shared_ptr<ActiveAnimation> anim){
    if (obj->getIsPlayer()){
        // send the event to notify that the player character is dead
        std::shared_ptr<Event> prEvent = PlayerRemovedEvent::alloc();
        this->notify(prEvent.get());
    }
    
    anim->getAnimationNode()->removeFromParent();
    state->removeObject(obj);
}

void AnimationController::dispose(){
    _worldnode = nullptr;
}

bool AnimationController::init(std::shared_ptr<GameState> state, const std::shared_ptr<GenericAssetManager>& assets) {
    _worldnode = state->getWorldNode();
    
    std::vector<std::shared_ptr<GameObject>> playerObjects = state->getPlayerCharacters();
    
    return true;
}
