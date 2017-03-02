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
#include <cugl/base/CUBase.h>
#include <cugl/2d/CUPathNode.h>

#define MIN_DISTANCE 13

using namespace cugl;

PathController::PathController(){}

void PathController::attach(std::shared_ptr<Observer> obs) {
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
void PathController::eventUpdate(Event* e) {}


void PathController::addPathToScene() {
	Poly2 pathPoly = _path->getPoly();
	auto pathNode = PathNode::allocWithPoly(pathPoly, 5, PathJoint::ROUND, PathCap::ROUND);
	printf("%i", _path->size());
	pathNode->setAnchor(Vec2::ANCHOR_MIDDLE);
	Vec2 midPoint = Vec2::Vec2((_minx + _maxx) / 2, (_miny + _maxy) / 2);
	pathNode->setPosition(midPoint);
	pathNode->setColor(Color4::CYAN);
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

void PathController::update(float timestep,std::shared_ptr<GameState> state){
	Vec2 position = Input::get<Mouse>()->pointerPosition();
	position.y = _height - position.y;
	bool isPressed = Input::get<Mouse>()->buttonDown().hasLeft();
	if (!_wasPressed && isPressed) {
		_path->clear();
		Vec2 currentLocation = state->getActiveCharacter()->getNode()->getPosition();
		_path->add(currentLocation);
		resetMinMax();
		updateMinMax(currentLocation);
	}
	if (isPressed) {
		Vec2 prev = _path->size() == 0 ? Vec2::Vec2(0, 0) : _path->getLast();
		double diffx = position.x - prev.x;
		double diffy = position.y - prev.y;
		double distance = std::sqrt((diffx * diffx) + (diffy * diffy));
		if (distance > 0) {
			_path->add(position);
			updateMinMax(position);
			addPathToScene();
		}
	}
	if (_wasPressed && !isPressed) {
		addPathToScene();
		std::shared_ptr<PathFinished> pathEvent = PathFinished::alloc(_path, state->getActiveCharacter());
		notify(pathEvent.get());
	}
	_wasPressed = isPressed;
}

bool PathController::init(std::shared_ptr<GameState> state) {
	_pathSceneNode = Node::alloc();
	_pathSceneNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	_pathSceneNode->setPosition(Vec2::ZERO);
	state->getScene()->addChild(_pathSceneNode, 2);
	_height = Application::get()->getDisplayHeight();
	resetMinMax();
	_path = Path::alloc();
	return true;
}
