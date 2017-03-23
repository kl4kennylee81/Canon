//
//  WaveData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AIData_hpp
#define AIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

enum class AIType : int {
	HOMING, PATH, STATIC
};

enum class PathType : int {
	HORIZONTAL, VERTICAL, CUSTOM, NONE
};

class AIData : public Data {
public:

	AIType _aiType;

	PathType _pathType;

	std::vector<cugl::Vec2> _path;

	AIData() {}

	bool init(AIType aiType, PathType pathType , std::vector<cugl::Vec2> path) {
		_aiType = aiType;
		_pathType = pathType;
		_path = path;
		return true;
	}

	static std::shared_ptr<AIData> alloc(AIType aiType, PathType pathType, std::vector<cugl::Vec2> path = std::vector<cugl::Vec2>()) {
		std::shared_ptr<AIData> result = std::make_shared<AIData>();
		return (result->init(aiType, pathType, path) ? result : nullptr);
	}

	virtual std::string serialize();

	virtual bool preload(const std::string& file);

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);

	virtual bool materialize();
};
#endif /* AIData_hpp */
