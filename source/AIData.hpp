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
#include "Util.h"

using std::string;

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

	PathType getPathTypeFromString(const std::string& str);

	cugl::Vec2 getTupleFromString(const std::string& s);

	AIType getTypeFromString(const std::string& str);

	std::vector<cugl::Vec2> getPathFromString(const std::string& str);

	bool init(string aiType, string pt, string p)
	{
		AIType type = getTypeFromString(aiType);
		PathType pathType = getPathTypeFromString(pt);
		std::vector<cugl::Vec2> path = getPathFromString(p);
		return init(type, pathType, path);
	}

	bool init(AIType aiType, PathType pathType, std::vector<cugl::Vec2> path) {
		_aiType = aiType;
		_pathType = pathType;
		_path = path;
		return true;
	}

	static std::shared_ptr<AIData> alloc(string aiType, string pathType, string path = "") {
		std::shared_ptr<AIData> result = std::make_shared<AIData>();
		return (result->init(aiType, pathType, path) ? result : nullptr);
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