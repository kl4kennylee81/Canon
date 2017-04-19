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

using namespace cugl;

CollisionController::CollisionController():
BaseController(){}
#define DEBUG_KEY KeyCode::D

#define BLUE_COLOR   Color4::BLUE
#define GOLD_COLOR   Color4::YELLOW
#define DEBUG_COLOR  Color4::GREEN
#define DEBUG_OFF_COLOR Color4::RED

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
    
    //remove duplicates
    sort(objsScheduledForRemoval.begin(), objsScheduledForRemoval.end() );
    objsScheduledForRemoval.erase(unique(objsScheduledForRemoval.begin(), objsScheduledForRemoval.end() ), objsScheduledForRemoval.end() );
    
    for (auto obj : objsScheduledForRemoval) {
        if (obj->getIsPlayer()){
            // TODO check if we actually need this inGame flag
            inGame = false;
        }
        removeFromWorld(obj);
    }
    
    // check for resized dirty obstacles that need to be remade
    for (std::shared_ptr<GameObject> gameObj : state->getEnemyObjects()){
        if (gameObj->getPhysicsComponent() == nullptr){
            continue;
        }
        
        if (gameObj->getPhysicsComponent()->getBody() == nullptr){
            continue;
        }
        if (gameObj->getPhysicsComponent()->getBody()->isDirty()){
            // create a new obstacle based on the scaling
            
            
            // delete the physics body and remake it
            removeFromWorld(gameObj.get());
        }
    }
    
    objsScheduledForRemoval.clear();
    
    if (inGame) {
        _world->update(timestep);
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

std::shared_ptr<PolygonObstacle> createObstacle(std::vector<float> vertices,float scaleX=1.0,float scaleY=1.0){
    // must be even number
    CUAssert(vertices.size() % 2 == 0);
    
    // scale the shape according to the size
    for (int i =0;i<vertices.size();i++){
        if (i % 2 == 0){
            vertices[i] = vertices[i] * scaleX;
        } else {
            vertices[i] = vertices[i] * scaleY;
        }
    }
    Poly2 poly(vertices);
    SimpleTriangulator triangulator;
    triangulator.set(poly);
    triangulator.calculate();
    poly.setIndices(triangulator.getTriangulation());
    auto obst = PolygonObstacle::alloc(poly);
    return obst;
}

void CollisionController::initPhysicsComponent(ObjectInitEvent* objectInit) {
    auto obst = createObstacle(objectInit->shapeData->vertices);
    obst->setPosition(objectInit->waveEntry->getPosition());
    
    std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(obst, objectInit->waveEntry->getElement());
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
    
    std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(obst, zoneInit->element);
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
    int remove = 0;
    
    if (obj1->getIsPlayer() && !obj2->getIsPlayer()) {
        if (sameElement) {
            remove = 2;
        } else {
            remove = 1;
        }
    }
    if (obj2->getIsPlayer() && !obj1->getIsPlayer()) {
        if (sameElement) {
            remove = 1;
        } else {
            remove = 2;
        }
    }
    
    if (remove == 1) {
        if (obj1->type == GameObject::ObjectType::ZONE) {
            return;
        }
        objsScheduledForRemoval.push_back(obj1);
        std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(obj1);
        notify(objectGoneEvent.get());
    }
    if (remove == 2) {
        if (obj2->type == GameObject::ObjectType::ZONE) {
            return;
        }
        objsScheduledForRemoval.push_back(obj2);
        std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(obj2);
        notify(objectGoneEvent.get());
    }
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    //dont need?
}
