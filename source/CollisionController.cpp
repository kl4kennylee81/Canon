//
//  CollisionController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "CollisionController.hpp"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

using namespace cugl;

CollisionController::CollisionController():
BaseController(){}

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
    
    std::vector<std::shared_ptr<GameObject>> playerChars = state->getPlayerCharacters();
    
    // iterate through player character gameObjects and create the PhysicsComponent
    // and add it to the ObstacleWorld
    for(auto it = playerChars.begin() ; it != playerChars.end(); ++it) {
        GameObject* obj = it->get();
        std::shared_ptr<BoxObstacle> box = BoxObstacle::alloc(obj->getNode()->getPosition(), obj->getNode()->getSize());
        std::shared_ptr<PhysicsComponent> physics = PhysicsComponent::alloc(box);
        obj->setPhysicsComponent(physics);
        _world->addObstacle(box);
    }
    
    return true;
}

void CollisionController::beginContact(b2Contact* contact) {
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}
