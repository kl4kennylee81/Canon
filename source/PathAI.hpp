//
//  HomingAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathAI_hpp
#define PathAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"
#include "AIData.hpp"
#include "PathAI.hpp"
#include "PathAIData.hpp"
#include "ActivePath.hpp"
#include "Path.hpp"

class PathAI : public ActiveAI {
private:
	PathType _type;
	std::shared_ptr<ActivePath> _activePath;
	PathDirection _direction;

public:
    
    PathAI(){}
    
    ~PathAI(){
        _object = nullptr;
        _activePath = nullptr;
    }

	static std::string PathAI::getStringFromPathType(const PathType pt)
	{
		if (pt == PathType::HORIZONTAL) return "HORIZONTAL";
		if (pt == PathType::VERTICAL) return "VERTICAL";
		if (pt == PathType::CUSTOM) return "CUSTOM";
		return "NONE";
	}

	void update(std::shared_ptr<GameState> state);

	bool garbageCollect(GameObject* obj);

	std::vector<std::shared_ptr<GameObject>> getObjects() {
		std::vector<std::shared_ptr<GameObject>> objects;
		objects.push_back(_object);
		return objects;
	}

	bool isActive();

	void toggleActive();

	bool init(std::shared_ptr<GameObject> object, PathType type, std::vector<cugl::Vec2> path, PathDirection direction);

	static std::shared_ptr<PathAI> alloc(std::shared_ptr<GameObject> object, PathType type, 
		std::vector<cugl::Vec2> path = std::vector<cugl::Vec2>(), PathDirection direction = PathDirection::RANDOM) 
	{
		std::shared_ptr<PathAI> result = std::make_shared<PathAI>();
		return (result->init(object, type, path, direction) ? result : nullptr);
	}

	std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	static std::shared_ptr<PathAI> allocWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state)
	{
		std::shared_ptr<PathAI> result = std::make_shared<PathAI>();
		return (result->initWithJson(json, state) ? result : nullptr);
	}

	bool initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state);
};

#endif /* PathAI_hpp */
