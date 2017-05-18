//
//  PathController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "PathController.hpp"
#include "Path.hpp"
#include "PathEvent.hpp"
#include "PathParameters.h"
#include <cugl/base/CUBase.h>
#include <cugl/2d/CUPathNode.h>
#include "MoveEvent.hpp"
#include "LevelEvent.hpp"
#include "InputController.hpp"
#include "Util.hpp"
#include "TexturedPathNode.hpp"

using namespace cugl;

/**
 * Each character has a cooldown of this much after each swipe
 */
#define SWIPE_COOLDOWN_FRAMES 10;

/*
 * Note: When a click event is received, the coordinates are taken in as screen coordinates ((0,0) at top left and bounds are
 * size of the device) but then the path is accumilated in world coordinates ((0,0) is bottom left and bounds are your GAME_WIDTH);
 * When sending out the path event, the path coordinates are in physics coordinates.
 */
PathController::PathController():
BaseController(){}

void PathController::attach(Observer* obs) {
	BaseController::attach(obs);
}

void PathController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void PathController::notify(Event* e) {
	BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void PathController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::MOVE:
        {
            MoveEvent* moveEvent = (MoveEvent*)e;
            switch(moveEvent->_moveEventType){
                case MoveEvent::MoveEventType::MOVE_FINISHED:
                {
                    // character is now done moving through the path
                    _movingChars.erase(moveEvent->_character);
                    controllerState = IDLE;
                    break;
                }
            }
            break;
        }
        case Event::EventType::LEVEL: {
            LevelEvent* levelEvent = (LevelEvent*)e;
            switch (levelEvent->levelEventType) {
                case LevelEvent::LevelEventType::OBJECT_SPAWN: {
                    _spawnStart = true;
                    break;
                }
            }
            break;
        }
    }
}


std::shared_ptr<GameObject> PathController::getActiveCharacter(std::shared_ptr<GameState> state) {
    std::shared_ptr<GameObject> active = state->getActiveCharacter();
    if (active == nullptr){
        return nullptr;
    }
    
    if(std::find(_movingChars.begin(), _movingChars.end(), active) != _movingChars.end()) {
        active = state->getOtherPlayer(active);
        if(std::find(_movingChars.begin(), _movingChars.end(), active) != _movingChars.end()){
            return nullptr;
        }
    }
    return active;
}

void PathController::addPathToScene(std::shared_ptr<GameState> state) {
	Poly2 pathPoly = _path->getPoly();
	auto pathNode = TexturedPathNode::allocWithPoly(pathPoly, 0.25, PathJoint::ROUND, PathCap::ROUND);
    pathNode->setPath(_path->clone());
    pathNode->setStroke(15);
    pathNode->setTexture(_mainTexture);
    pathNode->setCapTexture(_capTexture);
    pathNode->setAnchor(Vec2::ANCHOR_MIDDLE);
	Vec2 midPoint = Vec2::Vec2((_minx + _maxx) / 2, (_miny + _maxy) / 2);
	pathNode->setPosition(midPoint);
    
//    Poly2 pathPoly = _path->getPoly();
//    auto pathNode = PathNode::allocWithPoly(pathPoly, 0.5, PathJoint::ROUND, PathCap::ROUND);
//    pathNode->setAnchor(Vec2::ANCHOR_MIDDLE);
//    Vec2 midPoint = Vec2::Vec2((_minx + _maxx) / 2, (_miny + _maxy) / 2);
//    pathNode->setPosition(midPoint);
    
    if (_drawingChar == nullptr){
        return;
    }
    
    if (_drawingChar->getPhysicsComponent() == nullptr){
        return;
    }
    
    
    
    // switch color of path depending on who's turn
    if (_drawingChar->getPhysicsComponent()->getElementType() == ElementType::GOLD) {
        pathNode->setColor(Color4::ORANGE);
    } else {
        pathNode->setColor(Color4::BLUE);
    }
    
	_pathSceneNode->removeAllChildren();
	_pathSceneNode->addChild(pathNode, 2);
}

void PathController::resetMinMax() {
	_minx = FLT_MAX;
	_miny = FLT_MAX;
	_maxx = 0;
	_maxy = 0;
}

void PathController::updateMinMax(Vec2 vec) {
	_minx = std::min(_minx, vec.x);
	_miny = std::min(_miny, vec.y);
	_maxx = std::max(_maxx, vec.x);
	_maxy = std::max(_maxy, vec.y);
}

bool PathController::isOnCooldown() {
    return _cooldown_frames < SWIPE_COOLDOWN_FRAMES;

}

void PathController::update(float timestep,std::shared_ptr<GameState> state){
    if (!_spawnStart){
        return;
    }
    
    // can't start drawing a path before a character is done moving through a previous path
    //if (controllerState == MOVING) {
    //    return;
    //}

	// clear path on two finger touch
    if (InputController::getDoubleTouch() && controllerState != PathControllerState::IDLE) {
        controllerState = IDLE;
		_path->clear();
		_wasPressed = false;
		_pathSceneNode->removeAllChildren();
        std::shared_ptr<PathCancelled> drawEvent = PathCancelled::alloc();
        notify(drawEvent.get());
		return;
	}
    
    _cooldown_frames += GameState::_internalClock->getTimeDilation();
    bool isPressed = InputController::getIsPressed();
    Vec2 position = isPressed ? InputController::getInputVector() : Vec2::Vec2();
    
    Vec2 physicsPosition = Vec2::Vec2();
    
    if (isPressed){
        Util::screenToPhysicsCoords(position,physicsPosition);
        float buffer = GAME_PHYSICS_WIDTH * 0.02;
        float x2 = GAME_PHYSICS_WIDTH - buffer;
        float y2 = GAME_PHYSICS_HEIGHT - buffer;
        physicsPosition.clamp(Vec2::Vec2(buffer, buffer), Vec2::Vec2(x2, y2));
        
        Vec2 physicPosition = Vec2::Vec2();
        Util::screenToPhysicsCoords(position, physicPosition);
        
        if (controllerState == IDLE){
            state->setClosestChar(physicPosition);
        }
    }
    
	if (!_wasPressed && isPressed) {
        
        if (isOnCooldown()) {
            return;
        }
        
		_path->clear();
        
        std::shared_ptr<GameObject> active = getActiveCharacter(state);
        if (active == nullptr){
            return;
        }
        
        Vec2 currentLocation = active->getPosition();
        _drawingChar = active;
		
		// can't start drawing a path if the touch is far away from the active character
		if (physicsPosition.distance(currentLocation) > TOUCH_RADIUS) return;

		_path->add(currentLocation);
		resetMinMax();
		updateMinMax(currentLocation);
        
        // notify that the controller has started drawing
        std::shared_ptr<PathDrawing> drawEvent = PathDrawing::alloc();
        notify(drawEvent.get());
        
        controllerState = DRAWING;
	}
	if (isPressed) {
		Vec2 prev = _path->size() == 0 ? Vec2::Vec2(0, 0) : _path->getLast();
		double diffx = physicsPosition.x - prev.x;
		double diffy = physicsPosition.y - prev.y;
		double distance = std::sqrt((diffx * diffx) + (diffy * diffy));
		if (distance > MIN_DISTANCE) {
            _path->add(physicsPosition);
			updateMinMax(physicsPosition);
			addPathToScene(state);
		}
	}
	if (_wasPressed && !isPressed) {
		addPathToScene(state);
        
        // notify that the controller has finished drawing
        std::shared_ptr<PathFinished> pathEvent = PathFinished::alloc(_path, _drawingChar, state->getOtherPlayer(_drawingChar));
        _movingChars.insert(_drawingChar);
        notify(pathEvent.get());
        _pathSceneNode->removeAllChildren();
        
        controllerState = IDLE;
        _cooldown_frames = 0;
    }
	_wasPressed = isPressed;
}
    
bool PathController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
	_pathSceneNode = Node::alloc();
	_pathSceneNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_pathSceneNode->setPosition(Vec2::ZERO);
    
    // attach it to the world so automatically scales and translates
	state->getWorldNode()->addChild(_pathSceneNode, 2);
    
	_height = Application::get()->getDisplayHeight();
	resetMinMax();
	_path = Path::alloc();
    controllerState = IDLE;
	_wasPressed = false;
	_cooldown_frames = SWIPE_COOLDOWN_FRAMES;
    _world = world;
    
    _spawnStart = false;
    
    _capTexture = _world->getAssetManager()->get<Texture>("square");
    _mainTexture = _world->getAssetManager()->get<Texture>("square");

	return true;
}

void PathController::dispose(){
    _path = nullptr;
    _pathSceneNode = nullptr;
}
