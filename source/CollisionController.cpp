//
//  CollisionController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "CollisionController.hpp"
#include "Element.hpp"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

using namespace cugl;

CollisionController::CollisionController():
BaseController(){}
#define DEBUG_KEY KeyCode::D

#define BLUE_COLOR   Color4::BLUE
#define GOLD_COLOR   Color4::YELLOW

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
void CollisionController::eventUpdate(Event* e) {}

void CollisionController::update(float timestep,std::shared_ptr<GameState> state){
    Keyboard* keys = Input::get<Keyboard>();
    
    if (keys->keyPressed(DEBUG_KEY)) {
        setDebug(!isDebug());
    }
    _world->update(timestep);
}


bool CollisionController::init() {
	return true;
}
    
bool CollisionController::init(std::shared_ptr<GameState> state){
    Size size = Application::get()->getDisplaySize();
    Rect bounds = Rect::Rect(0,0,size.getIWidth(),size.getIHeight());
    
    // 2nd arguement is setting gravity to 0
    _world = cugl::ObstacleWorld::alloc(bounds,Vec2::ZERO);
    _world->setGravity(Vec2::ZERO);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
        beforeSolve(contact,oldManifold);
    };
    
    _debugnode = state->getDebugNode();
    
    std::vector<std::shared_ptr<GameObject>> allObjects = state->getAllObjects();
    
    // iterate through player character gameObjects and create the PhysicsComponent
    // and add it to the ObstacleWorld
    for(auto it = allObjects.begin() ; it != allObjects.end(); ++it) {
        addToWorld(it->get());
    }
    
    Input::activate<Keyboard>();
    setDebug(false);
    
    
    
    return true;
}

bool CollisionController::addToWorld(GameObject* obj) {
    auto obst = obj->getPhysicsComponent()->getBody();
    
    if (obj->getPhysicsComponent()->getElementType() == Element::BLUE) {
        obst->setDebugColor(BLUE_COLOR);
    }
    if (obj->getPhysicsComponent()->getElementType() == Element::GOLD) {
        obst->setDebugColor(GOLD_COLOR);
    }
    
    _world->addObstacle(obst);
    obst->setDebugScene(_debugnode);
    obst->getBody()->SetUserData(obj);
    
    return true;
}

void CollisionController::beginContact(b2Contact* contact) {
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    auto gameobj1 = body1->GetUserData();
    auto gameobj2 = body2->GetUserData();
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}
