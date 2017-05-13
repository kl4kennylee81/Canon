#include "PathAI.hpp"
#include "MoveController.hpp"
#include "GameState.hpp"
#include "MoveController.hpp"
#include "PathParameters.h"
#include "PathAIData.hpp"
#include <random>

using namespace cugl;

void PathAI::update(std::shared_ptr<GameState> state) {
	int index = _activePath->_pathIndex;
	Vec2 goal = _activePath->_path->get(index);
	Vec2 current = _object->getPosition();
	if (std::abs(current.distance(goal)) <= (RADIUS)) {
		int newIndex = (index + 1) % _activePath->_path->size();
		_activePath->_pathIndex = newIndex;
		goal = _activePath->_path->get(newIndex);
		switch (_direction) {
			case PathDirection::LEFT:
				_direction = PathDirection::RIGHT;
				break;
			case PathDirection::RIGHT:
				_direction = PathDirection::LEFT;
				break;
			case PathDirection::UP:
				_direction = PathDirection::DOWN;
				break;
			case PathDirection::DOWN:
			default:
				_direction = PathDirection::UP;
				break;
		}
	}

    auto physics = _object->getPhysicsComponent();
    float speed = physics->getSpeed() == 0 ? AI_SPEED * 60 : physics->getSpeed();
	Vec2 velocity = MoveController::getVelocityVector(current, goal, speed);
	_object->getPhysicsComponent()->getBody()->setLinearVelocity(velocity);

}

bool PathAI::garbageCollect(GameObject* obj) {
	return true;
}

bool PathAI::isActive() {
	return _isActive;
}

bool PathAI::init(std::shared_ptr<GameObject> object, PathType type, std::vector<cugl::Vec2> path, PathDirection direction) {
	_object = object;
	_isActive = false;
	_type = type;
	_direction = direction;
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
	switch (type) {
	case PathType::VERTICAL:
	{
		Vec2 position = object->getPosition();
        float height = GAME_PHYSICS_HEIGHT;
		Vec2 top = Vec2::Vec2(position.x, height);
		Vec2 bottom = Vec2::Vec2(position.x, 0);
		auto newPath = Path::alloc();
		if (_direction == PathDirection::UP || (_direction == PathDirection::RANDOM && dist2(rng) == 1)) {
			newPath->add(top);
			newPath->add(bottom);
		}
		else {
			newPath->add(bottom);
			newPath->add(top);
		}
		_activePath = ActivePath::alloc(newPath);
		break;
	}
	case PathType::HORIZONTAL:
	{
		Vec2 position = object->getPosition();
        float width = GAME_PHYSICS_WIDTH;
		Vec2 right = Vec2::Vec2(width, position.y);
		Vec2 left = Vec2::Vec2(0, position.y);
		auto newPath = Path::alloc();
		if (_direction == PathDirection::RIGHT || (_direction == PathDirection::RANDOM && dist2(rng) == 1)) {
			newPath->add(right);
			newPath->add(left);
		}
		else {
			newPath->add(left);
			newPath->add(right);
		}
		_activePath = ActivePath::alloc(newPath);
		break;
	}
	case PathType::CUSTOM: 
	{
		auto newPath = Path::alloc();
		for (auto it : path) {
			Vec2 point = Vec2::Vec2(it.x, it.y);
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

std::shared_ptr<JsonValue> PathAI::toJsonValue() {
	std::shared_ptr<JsonValue> pathJson = JsonValue::allocObject();

	// duplicate code from move controller rip
	pathJson->appendChild("pathIndex", JsonValue::alloc(std::to_string(_activePath->_pathIndex)));
	pathJson->appendChild("pathVertices", _activePath->_path->toJsonValue());
	pathJson->appendChild("pathType", JsonValue::alloc(getStringFromPathType(_type)));
	pathJson->appendChild("pathDirection", JsonValue::alloc(PathAIData::getStringFromPathDirection(_direction)));

	pathJson->appendChild("uid", JsonValue::alloc(std::to_string(_object->getUid())));
	pathJson->appendChild("aiType", cugl::JsonValue::alloc("PATH"));
	return pathJson;
}

bool PathAI::initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state) {
	PathType pType = PathAIData::getPathTypeFromString(json->getString("pathType"));
	PathDirection dir = PathAIData::getPathDirectionFromString(json->getString("pathDirection"));

	std::vector<Vec2> path;
	for (int i = 0; i < json->get("pathVertices")->size(); i++) {
		std::shared_ptr<JsonValue> vec = json->get("pathVertices")->get(i);
		path.push_back(Vec2(vec->getFloat("x"), vec->getFloat("y")));
	}

	int uid = std::stoi(json->getString("uid"));
	std::shared_ptr<GameObject> gObj = state->getUID2GameObject(uid);

	init(gObj, pType, path, dir);
	return true;

}
