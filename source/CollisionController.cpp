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

CollisionController::CollisionController(){}

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

void CollisionController::update(float timestep){
}


bool CollisionController::init() {
	return true;
}
    
bool CollisionController::init(const cugl::Rect& bounds){
    _world = cugl::ObstacleWorld::alloc(bounds);
    _world->setGravity(Vec2::ZERO);
    return true;
}
