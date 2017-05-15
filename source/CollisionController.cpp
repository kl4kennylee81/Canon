//
//  CollisionController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "CollisionController.hpp"
#include "Element.hpp"
#include "CollisionEvent.hpp"
#include "LevelEvent.hpp"
#include "ZoneEvent.hpp"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "BulletSpawnEvent.hpp"
#include <list>

using namespace cugl;

CollisionController::CollisionController():
BaseController(){}
#define DEBUG_KEY KeyCode::D

#define BLUE_COLOR   Color4::BLUE
#define GOLD_COLOR   Color4::YELLOW
#define DEBUG_COLOR  Color4::GREEN
#define DEBUG_OFF_COLOR Color4::RED

#define HIT_STUN 90

void CollisionController::attach(Observer* obs) {
	BaseController::attach(obs);
}
void CollisionController::detach(Observer* obs) {
	BaseController::detach(obs);
}
void CollisionController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void CollisionController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_INIT: {
                    ObjectInitEvent* objectInit = (ObjectInitEvent*)levelEvent;
                    initPhysicsComponent(objectInit);
                    if (objectInit->object->getIsPlayer() && objectInit->object->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        bluePlayer = objectInit->object.get();
                    }
                    if (objectInit->object->getIsPlayer() && objectInit->object->getPhysicsComponent()->getElementType() == ElementType::GOLD) {
                        goldPlayer = objectInit->object.get();
                    }
                    break;
                }
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    ObjectSpawnEvent* objectSpawn = (ObjectSpawnEvent*)levelEvent;
                    GameObject* obj = objectSpawn->object.get();
                    addToWorld(obj);
                    break;
                }
            }
            break;
        }
        case Event::EventType::ZONE: {
            ZoneEvent* zoneEvent = (ZoneEvent*)e;
            switch (zoneEvent->zoneEventType) {
                case ZoneEvent::ZoneEventType::ZONE_INIT: {
                    ZoneInitEvent* zoneInit = (ZoneInitEvent*)zoneEvent;
                    initPhysicsComponent(zoneInit);
                    if (zoneInit->object->getIsPlayer() && zoneInit->object->getPhysicsComponent()->getElementType() == ElementType::BLUE) {
                        blueZone = zoneInit->object.get();
                    }
                    if (zoneInit->object->getIsPlayer() && zoneInit->object->getPhysicsComponent()->getElementType() == ElementType::GOLD) {
                        goldZone = zoneInit->object.get();
                    }
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_SPAWN: {
                    ZoneSpawnEvent* zoneSpawn = (ZoneSpawnEvent*)zoneEvent;
                    addToWorld(zoneSpawn->object);
                    zoneSpawn->object->getPhysicsComponent()->getBody()->setDebugColor(DEBUG_OFF_COLOR);
                    objsToIgnore.push_back(zoneSpawn->object);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_ON: {
                    ZoneOnEvent* zoneOn = (ZoneOnEvent*)zoneEvent;
                    zoneOn->object->getPhysicsComponent()->getBody()->setDebugColor(DEBUG_COLOR);
                    auto position = std::find(objsToIgnore.begin(), objsToIgnore.end(), zoneOn->object);
                    if (position != objsToIgnore.end()) objsToIgnore.erase(position);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_OFF: {
                    ZoneOffEvent* zoneOff = (ZoneOffEvent*)zoneEvent;
                    zoneOff->object->getPhysicsComponent()->getBody()->setDebugColor(DEBUG_OFF_COLOR);
                    objsToIgnore.push_back(zoneOff->object);
                    break;
                }
                case ZoneEvent::ZoneEventType::ZONE_DELETE: {
                    ZoneDeleteEvent* zoneDelete = (ZoneDeleteEvent*)zoneEvent;
                    removeFromWorld(zoneDelete->object);
                    break;
                }
            }
            break;
        }
    }
}

void CollisionController::update(float timestep,std::shared_ptr<GameState> state){
    Keyboard* keys = Input::get<Keyboard>();
    if (keys->keyPressed(DEBUG_KEY)) {
        setDebug(!isDebug());
    }
    
    updateHitStun();
    
    //remove duplicates
    sort(objsScheduledForRemoval.begin(), objsScheduledForRemoval.end() );
    objsScheduledForRemoval.erase(unique(objsScheduledForRemoval.begin(), objsScheduledForRemoval.end() ), objsScheduledForRemoval.end() );
    
    for (auto obj : objsScheduledForRemoval) {
        if (obj->getIsPlayer()){
            // TODO check if we actually need this inGame flag
            // added so things dont keep moving during death animation
            inGame = false;
        }
        removeFromWorld(obj);
    }
    
    objsScheduledForRemoval.clear();
    
    if (inGame) {
        _world->update(timestep);
        for(auto it: _world->getObstacles()) {
            GameObject* obj = static_cast<GameObject*>(it->getBody()->GetUserData());
            if(obj->type == GameObject::ObjectType::BULLET){
                Vec2 pos = it->getPosition();
                if(pos.x < -2 || pos.y < -2 || pos.x > GAME_PHYSICS_WIDTH + 2 || pos.y > GAME_PHYSICS_HEIGHT + 2) {
                    objsScheduledForRemoval.push_back(obj);
                    std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(obj);
                    notify(objectGoneEvent.get());
                }
            }
        }
        
        // update all obstacle after updating the world
        // check for resized dirty obstacles that need to be remade
        for (std::shared_ptr<GameObject> gameObj : state->getEnemyObjects()){
            if (gameObj->getPhysicsComponent() == nullptr){
                continue;
            }
            
            if (gameObj->getPhysicsComponent()->getBody() == nullptr){
                continue;
            }
            gameObj->getPhysicsComponent()->getBody()->update(timestep);
        }
        
        // check for resized dirty obstacles that need to be remade
        for (std::shared_ptr<GameObject> gameObj : state->getPlayerCharacters()){
            if (gameObj->getPhysicsComponent() == nullptr){
                continue;
            }
            
            if (gameObj->getPhysicsComponent()->getBody() == nullptr){
                continue;
            }
            gameObj->getPhysicsComponent()->getBody()->update(timestep);
        }
    }
}

void CollisionController::updateHitStun(){
    auto itr = hitStunMap.begin();
    while (itr != hitStunMap.end()) {
        itr->second -= GameState::_internalClock->getTimeDilation();
        if (itr->second <= 0) {
            //remove from ignore list
            auto position = std::find(objsToIgnore.begin(), objsToIgnore.end(), itr->first);
            if (position != objsToIgnore.end()) objsToIgnore.erase(position);
            
            //send event
            if(itr->first->type == GameObject::ObjectType::CHARACTER) {
                std::shared_ptr<ObjectHitFinishedEvent> objectHitFinishedEvent = ObjectHitFinishedEvent::alloc(itr->first);
                notify(objectHitFinishedEvent.get());
            }
            
            itr = hitStunMap.erase(itr);
        } else {
            ++itr;
        }
    }
}

void CollisionController::dispose(){
    _world = nullptr;
    _debugnode = nullptr;
}


bool CollisionController::init() {
	return true;
}
    
bool CollisionController::init(std::shared_ptr<GameState> state){
    Rect bounds = state->getRect();
    
    // 2nd arguement is setting gravity to 0
    _world = cugl::ObstacleWorld::alloc(bounds,Vec2::ZERO);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
        beforeSolve(contact,oldManifold);
    };
    
    // keep the world update to 60 fps without lockstep it simulates up until it is called again
    // which is not garunteed to be within our set timeframe for example when using the debugger.
    _world->setLockStep(true);
    _world->setStepsize(1.f/Application::get()->getFPS());
    
    _debugnode = state->getDebugNode();
    
    std::vector<std::shared_ptr<GameObject>> enemyObjects = state->getEnemyObjects();

    std::vector<std::shared_ptr<GameObject>> playerObjects = state->getPlayerCharacters();
    
    // iterate through player character gameObjects and create the PhysicsComponent
    // and add it to the ObstacleWorld
    for(auto it = playerObjects.begin() ; it != playerObjects.end(); ++it) {
        addToWorld(it->get());
    }
    
    // iterate through enemy character gameObjects and create the PhysicsComponent
    // and add it to the ObstacleWorld
    for(auto it = enemyObjects.begin() ; it != enemyObjects.end(); ++it) {
        addToWorld(it->get());
    }
    
    Input::activate<Keyboard>();
    setDebug(false);
    
    inGame = true;
    
    return true;
}

void CollisionController::initPhysicsComponent(ObjectInitEvent* objectInit) {
    Poly2 poly(objectInit->shapeData->vertices);
    SimpleTriangulator triangulator;
    triangulator.set(poly);
    triangulator.calculate();
    poly.setIndices(triangulator.getTriangulation());
    auto obst = PolygonObstacle::alloc(poly);
    obst->setPosition(objectInit->waveEntry->getPosition());
    std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(obst, objectInit->waveEntry->getElement(),objectInit->objectData->getHealth());
    physics->setSpeed(objectInit->objectData->getSpeed());
    objectInit->object->setPhysicsComponent(physics);
}

void CollisionController::initPhysicsComponent(ZoneInitEvent* zoneInit) {
    Poly2 poly(zoneInit->shapeData->vertices);
    SimpleTriangulator triangulator;
    triangulator.set(poly);
    triangulator.calculate();
    poly.setIndices(triangulator.getTriangulation());
    auto obst = PolygonObstacle::alloc(poly);
    obst->setPosition(zoneInit->pos);
    std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(obst, zoneInit->element,1);
    zoneInit->object->setPhysicsComponent(physics);
}

bool CollisionController::addToWorld(GameObject* obj) {
    auto obst = obj->getPhysicsComponent()->getBody();
    obst->setDebugColor(DEBUG_COLOR);
    obst->setSensor(true);
    
    _world->addObstacle(obst);
    obst->setDebugScene(_debugnode);
    obst->getBody()->SetUserData(obj);
    
    return true;
}

bool CollisionController::removeFromWorld(GameObject* obj) {
    _world->removeObstacle(obj->getPhysicsComponent()->getBody().get());
    
    obj->getPhysicsComponent()->getBody()->setDebugScene(nullptr);
    return true;
}

void CollisionController::beginContact(b2Contact* contact) {
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    auto obj1 = static_cast<GameObject*>(body1->GetUserData());
    auto obj2 = static_cast<GameObject*>(body2->GetUserData());
    
    if (std::find(objsToIgnore.begin(), objsToIgnore.end(), obj1) != objsToIgnore.end() || std::find(objsToIgnore.begin(), objsToIgnore.end(), obj2) != objsToIgnore.end())
    {
        return;
    }
    
    bool sameElement = (obj1->getPhysicsComponent()->getElementType() ==
                        obj2->getPhysicsComponent()->getElementType());
    
    std::list<GameObject*> removes;
    
    if (obj1->getIsPlayer() && !obj2->getIsPlayer()) {
        if (sameElement) {
            removes.push_back(obj2);
        } else {
            removes.push_back(obj1);
        }
    }
    if (obj2->getIsPlayer() && !obj1->getIsPlayer()) {
        if (sameElement) {
            removes.push_back(obj1);
        } else {
            removes.push_back(obj2);
        }
    }
    
    // player can kill each other
    if (obj1->getIsPlayer() && obj2->getIsPlayer()){
        // don't let them kill each other just with zones
        if (obj1->type == GameObject::ObjectType::ZONE || obj2->type == GameObject::ObjectType::ZONE) {
            return;
        }
        if (!sameElement){
            removes.push_back(bluePlayer);
            removes.push_back(goldPlayer);
        }
    }
    
    if (removes.size() == 0) {
        return;
    }
    for (GameObject* gotHit : removes){
        
        if (gotHit->type == GameObject::ObjectType::ZONE) {
            return;
        }
        gotHit->getPhysicsComponent()->getHit();
        if (gotHit->getPhysicsComponent()->isAlive()) {
            objsToIgnore.push_back(gotHit);
            hitStunMap.insert({gotHit,HIT_STUN});
            if (gotHit == bluePlayer) {
                objsToIgnore.push_back(blueZone);
                hitStunMap.insert({blueZone,HIT_STUN});
            }
            if (gotHit == goldPlayer) {
                objsToIgnore.push_back(goldZone);
                hitStunMap.insert({goldZone,HIT_STUN});
            }
            std::shared_ptr<ObjectHitEvent> objectHitEvent = ObjectHitEvent::alloc(gotHit);
            notify(objectHitEvent.get());
        } else {
            objsScheduledForRemoval.push_back(gotHit);
            std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(gotHit);
            notify(objectGoneEvent.get());
        }
    }
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    //dont need?
}
