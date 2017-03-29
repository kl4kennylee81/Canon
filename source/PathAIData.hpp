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

	std::string serialize() override;

	bool preload(const std::string& file) override;

	bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool materialize() override;

	std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;
};

#endif /* PathAIData.hpp */
