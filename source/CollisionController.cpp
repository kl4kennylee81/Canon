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

#define DEBUG_KEY KeyCode::D

#define BASIC_DENSITY       0.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION      0.1f
/** Friction of the crate objects */
#define BASIC_DAMPING       1.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f
/** Color to outline the physics nodes */
#define STATIC_COLOR    Color4::YELLOW
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR   Color4::GREEN

/** The positions of the crate pyramid */
float BOXES[] = { 14.5f, 14.25f,
    13.0f, 12.00f, 16.0f, 12.00f,
    11.5f,  9.75f, 14.5f,  9.75f, 17.5f, 9.75f,
    13.0f,  7.50f, 16.0f,  7.50f,
    11.5f,  5.25f, 14.5f,  5.25f, 17.5f, 5.25f,
    10.0f,  3.00f, 13.0f,  3.00f, 16.0f, 3.00f, 19.0f, 3.0f};

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

void CollisionController::update(float timestep,std::shared_ptr<GameState> state){
    Keyboard* keys = Input::get<Keyboard>();
    
    if (keys->keyPressed(DEBUG_KEY)) {
        setDebug(!isDebug());
    }
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
        GameObject* obj = it->get();
        auto obst = obj->getPhysicsComponent()->getBody();
        
        obst->setDebugColor(DYNAMIC_COLOR);
        obst->setDensity(BASIC_DENSITY);
        obst->setFriction(BASIC_FRICTION);
        obst->setAngularDamping(BASIC_DAMPING);
        obst->setRestitution(BASIC_RESTITUTION);
        
        _world->addObstacle(obst);
        obst->setDebugScene(_debugnode);
    }
    
    Input::activate<Keyboard>();
    setDebug(false);
    
    return true;
}

void CollisionController::beginContact(b2Contact* contact) {
}

void CollisionController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}
