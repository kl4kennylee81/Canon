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
#include "BulletSpawnEvent.hpp"
#include <cugl/cugl.h>

using namespace cugl;

#define BLUE_COLOR   Color4::CYAN
#define GOLD_COLOR   Color4::YELLOW
#define DEBUG_COLOR  Color4::GREEN

/** The alpha of the zones on/off */
#define ZONE_ON_ALPHA 0.5
#define ZONE_OFF_ALPHA 0.1

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
                case CollisionEvent::CollisionEventType::OBJECT_GONE: {
                    ObjectGoneEvent* objectGone = (ObjectGoneEvent*)collisionEvent;
                    GameObject* obj = objectGone->_obj;
                    handleAction(obj, AnimationAction::DEATH);
                    animationMap.at(obj)->setLastAnimation();
                    break;
                }
                case CollisionEvent::CollisionEventType::OBJECT_HIT: {
                    ObjectHitEvent* objectHit = (ObjectHitEvent*)collisionEvent;
                    GameObject* obj = objectHit->_obj;
                    if(obj->type != GameObject::ObjectType::ZONE){
                        animationMap.at(obj)->setHitStun(true);
                    }
                    handleAction(obj, AnimationAction::HIT);
                    break;
                }
                case CollisionEvent::CollisionEventType::HIT_FINISHED: {
                    ObjectHitFinishedEvent* objectHitFinished = (ObjectHitFinishedEvent*)collisionEvent;
                    GameObject* obj = objectHitFinished->_obj;
                    animationMap.at(obj)->setHitStun(false);
                    animationMap.at(obj)->getAnimationNode()->setColor(Color4::WHITE);
                    break;
                }
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
        case Event::EventType::BULLET_SPAWN: {
            BulletSpawnEvent* bulletSpawn = (BulletSpawnEvent*)e;
            GameObject* obj = bulletSpawn->object.get();
            handleAction(obj, AnimationAction::SPAWN);
            break;
        }
        case Event::EventType::PATH: {
            PathEvent* pathEvent = (PathEvent*)e;
            switch (pathEvent->_pathType) {
                case PathEvent::PathEventType::PATH_FINISHED:
                    PathFinished* pathFinished = (PathFinished*)pathEvent;
                    GameObject* obj = pathFinished->_activeChar.get();
                    handleAction(obj, AnimationAction::ATTACK);
                    break;
            }
            break;
        }
        case Event::EventType::MOVE: {
            MoveEvent* moveEvent = (MoveEvent*)e;
            switch(moveEvent->_moveEventType){
                case MoveEvent::MoveEventType::MOVE_FINISHED:
                {
                    GameObject* obj = moveEvent->_character.get();
                    handleAction(obj, AnimationAction::RETURN);
                    break;
                }
            }
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
                case ZoneEvent::ZoneEventType::ZONE_SPAWNING: {
                    ZoneSpawningEvent* zoneSpawning = (ZoneSpawningEvent*)zoneEvent;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(zoneSpawning->object)->getAnimationNode();
                    
                    // TODO change the colors to a macro
                    if (zoneSpawning->object->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        anim->setColor(Color4f(BLUE_COLOR));
                    } else {
                        anim->setColor(Color4f(GOLD_COLOR));
                    }
                    handleAction(zoneSpawning->object, AnimationAction::SPAWNING);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_SPAWN: {
                    ZoneSpawnEvent* zoneSpawn = (ZoneSpawnEvent*)zoneEvent;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(zoneSpawn->object)->getAnimationNode();
                    
                    // TODO change the colors to a macro
                    if (zoneSpawn->object->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        anim->setColor(Color4f(BLUE_COLOR));
                    } else {
                        anim->setColor(Color4f(GOLD_COLOR));
                    }
                    handleAction(zoneSpawn->object, AnimationAction::SPAWN);

                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_ON: {
                    ZoneOnEvent* zoneOn = (ZoneOnEvent*)zoneEvent;
                    GameObject* obj = zoneOn->object;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(obj)->getAnimationNode();
                    
                    animationMap.at(zoneOn->object)->setFlash(false);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_OFF: {
                    ZoneOffEvent* zoneOff = (ZoneOffEvent*)zoneEvent;
                    GameObject* obj = zoneOff->object;
                    std::shared_ptr<AnimationNode> anim = animationMap.at(obj)->getAnimationNode();
                    
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
    if (obj == nullptr){
        return;
    }
    std::shared_ptr<ActiveAnimation> anim = animationMap.at(obj);
    if (anim == nullptr){
        return;
    }
    
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
    
    std::shared_ptr<AnimationState> state = anim->getAnimationState();
    Color4 color = anim->getAnimationNode()->getColor();
    color.a = state->alpha * 255;
    anim->getAnimationNode()->setColor(color);
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
        
        GameObject* obj = it->first;
        if(obj->getPhysicsComponent()->hasArrow()){
            obj->syncArrow(state->getGameplayNode());
        }
        
        std::shared_ptr<ActiveAnimation> anim = it->second;
        
        if (anim->getAnimationNode()->getParent() == nullptr){
            it++;
            continue;
        }
        
        if (anim->getFlash()){
            anim->flashIndex += GameState::_internalClock->getTimeDilation();
            if (anim->flashIndex >= 30) {
                anim->flashIndex = 0;
                anim->lit = !anim->lit;
            }
            
            float alphascale = anim->lit ? ZONE_ON_ALPHA : ZONE_OFF_ALPHA;
            
            if (it->first->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                anim->getAnimationNode()->setColor(Color4f(BLUE_COLOR)*Color4f(1,1,1,alphascale));
            } else {
                anim->getAnimationNode()->setColor(Color4f(GOLD_COLOR)*Color4f(1,1,1,alphascale));
            }
            
        }
        
        if (anim->getHitStun()){
            anim->flashIndex += GameState::_internalClock->getTimeDilation();
            if (anim->flashIndex >= 10) {
                anim->flashIndex = 0;
                anim->lit = !anim->lit;
            }
            
            Color4 color = anim->lit ? Color4::WHITE : Color4::RED;
            anim->getAnimationNode()->setColor(color);
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
        switch(obj->type){
            case GameObject::ObjectType::CHARACTER:
            {
                // send the event to notify that the player character is dead
                std::shared_ptr<Event> prEvent = PlayerRemovedEvent::alloc();
                this->notify(prEvent.get());
                break;
            }
            case GameObject::ObjectType::ZONE:
            {
                break;
            }
            break;
            
        }
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

std::string AnimationController::serialize() {
	return toJsonValue()->toString();
}

std::shared_ptr<JsonValue> AnimationController::toJsonValue() {
	std::shared_ptr<JsonValue> fullJson = JsonValue::allocObject();
	return fullJson;
}
