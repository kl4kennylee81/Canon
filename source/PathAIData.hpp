//
//  PathAIData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathAIData_hpp
#define PathAIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AIData.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

enum class PathType : int {
	HORIZONTAL, VERTICAL, CUSTOM, NONE
};

enum class PathDirection : int {
	LEFT, RIGHT, UP, DOWN, RANDOM
};

class PathAIData : public AIData {
public:

	PathType _pathType;

	PathDirection _direction;

	std::vector<cugl::Vec2> _path;

	PathAIData() : AIData() {}

    bool init(PathType pathType, std::vector<cugl::Vec2> path, PathDirection direction);

	static std::shared_ptr<PathAIData> alloc(PathType pathType, std::vector<cugl::Vec2> path, PathDirection direction) {
		std::shared_ptr<PathAIData> result = std::make_shared<PathAIData>();
		return (result->init(pathType, path, direction) ? result : nullptr);
	}

	std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	bool preload(const std::string& file) override;

	bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool materialize() override;

	std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;

	static std::string getStringFromPathType(PathType pt) {
		switch (pt) {
			case PathType::CUSTOM:
				return "CUSTOM";
			case PathType::HORIZONTAL:
				return "HORIZONTAL";
			case PathType::NONE:
				return "NONE";
			case PathType::VERTICAL:
				return "VERTICAL";
			default:
				return "NONE";
		}
	}

	static PathType getPathTypeFromString(std::string s) {
		if (s == "CUSTOM") return PathType::CUSTOM;
		if (s == "HORIZONTAL") return PathType::HORIZONTAL;
		if (s == "NONE") return PathType::NONE;
		if (s == "VERTICAL") return PathType::VERTICAL;
		return PathType::NONE;
	}

	static std::string getStringFromPathDirection(PathDirection dir) {
		switch (dir) {
			case PathDirection::LEFT:
				return "LEFT";
			case PathDirection::RIGHT:
				return "RIGHT";
			case PathDirection::UP:
				return "UP";
			case PathDirection::RANDOM:
				return "RANDOM";
			case PathDirection::DOWN:
				return "DOWN";
			default:
				return "RANDOM";
		}
	}

	static PathDirection getPathDirectionFromString(std::string s) {
		if (s == "LEFT") return PathDirection::LEFT;
		if (s == "RIGHT") return PathDirection::RIGHT;
		if (s == "UP") return PathDirection::UP;
		if (s == "DOWN") return PathDirection::DOWN;
		if (s == "RANDOM") return PathDirection::RANDOM;
		return PathDirection::UP;
	}
};

#endif /* PathAIData.hpp */
