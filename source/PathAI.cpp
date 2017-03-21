#include "PathAI.hpp"
#include "MoveController.hpp"
#include "GameState.hpp"
#include "MoveController.hpp"
#include "PathParameters.h"

using namespace cugl;

void PathAI::update(std::shared_ptr<GameState> state) {
	int index = _activePath->_pathIndex;
	Vec2 goal = _activePath->_path->get(index);
	Vec2 current = _object->getPosition();
	if (std::abs(current.distance(goal)) <= (RADIUS / GameState::_physicsScale)) {
		int newIndex = (index + 1) % _activePath->_path->size();
		_activePath->_pathIndex = newIndex;
		goal = _activePath->_path->get(newIndex);
	}
	Vec2 velocity = MoveController::getVelocityVector(current, goal, AI_SPEED * 60 / GameState::_physicsScale);
	_object->getPhysicsComponent()->getBody()->setLinearVelocity(velocity);
}

bool PathAI::garbageCollect(GameObject* obj) {
	return true;
}

bool PathAI::isActive() {
	return _isActive;
}

bool PathAI::init(std::shared_ptr<GameObject> object, PathType type, std::vector<cugl::Vec2> path) {
	_object = object;
	_isActive = false;
	_type = type;
	switch (type) {
	case PathType::VERTICAL:
	{
		Vec2 position = object->getPosition();
		float height = GAME_WIDTH * GAME_ASPECT;
		Vec2 top = Vec2::Vec2(position.x, height / GameState::_physicsScale);
		Vec2 bottom = Vec2::Vec2(position.x, 0);
		auto newPath = Path::alloc();
		newPath->add(top);
		newPath->add(bottom);
		_activePath = ActivePath::alloc(newPath);
		break;
	}
	case PathType::HORIZONTAL:
	{
		Vec2 position = object->getPosition();
		float width = GAME_WIDTH;
		Vec2 top = Vec2::Vec2(width / GameState::_physicsScale, position.y);
		Vec2 bottom = Vec2::Vec2(0, position.y);
		auto newPath = Path::alloc();
		newPath->add(top);
		newPath->add(bottom);
		_activePath = ActivePath::alloc(newPath);
		break;
	}
	case PathType::CUSTOM: 
	{
		auto newPath = Path::alloc();
		for (auto it : path) {
			float scale = GameState::_physicsScale;
			Vec2 point = Vec2::Vec2(it.x / scale, it.y / scale);
			Vec2 translated = point.add(object->getPosition());
			newPath->add(translated);
		}
		_activePath = ActivePath::alloc(newPath);
		break;
	}
	}
	return true;
}

void PathAI::toggleActive() {
	_isActive = !_isActive;
}
