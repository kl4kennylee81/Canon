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

void CollisionController::attach(std::shared_ptr<Observer> obs) {
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
        if(!obj->getIsPlayer()) {
            removeFromWorld(state, obj);
            
            // TODO : temporary reset after losing
            state->reset = true;
        }
    }
    objsScheduledForRemoval.clear();
    
    _world->update(timestep);
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
    setDebug(true);
    
    return true;
}

void CollisionController::initPhysicsComponent(ObjectInitEvent* objectInit) {
    Poly2 poly(objectInit->shapeData->vertices);
    SimpleTriangulator triangulator;
    triangulator.set(poly);
    triangulator.calculate();
    poly.setIndices(triangulator.getTriangulation());
    auto obst = PolygonObstacle::alloc(poly);
    obst->setPosition(objectInit->waveEntry->position);
    
    std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(obst, objectInit->waveEntry->element);
    objectInit->object->setPhysicsComponent(physics);
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

bool CollisionController::removeFromWorld(std::shared_ptr<GameState> state, GameObject* obj) {
    _world->removeObstacle(obj->getPhysicsComponent()->getBody().get());
    
    obj->getPhysicsComponent()->getBody()->setDebugScene(nullptr);
    return true;
}

void CollisionController::beginContact(b2Contact* contact) {
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    auto obj1 = static_cast<GameObject*>(body1->GetUserData());
    auto obj2 = static_cast<GameObject*>(body2->GetUserData());
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
        objsScheduledForRemoval.push_back(obj1);
        std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(obj1);
        notify(objectGoneEvent.get());
        
    }
    if (remove == 2) {
        objsScheduledForRemoval.push_back(obj2);
        std::shared_ptr<ObjectGoneEvent> objectGoneEvent = ObjectGoneEvent::alloc(obj2);
        notify(objectGoneEvent.get());
    }
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    //dont need?
}
