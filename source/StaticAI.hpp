//
//  HomingAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef StaticAI_hpp
#define StaticAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class StaticAI : public ActiveAI {

public:
    
    StaticAI(){}
    
    ~StaticAI(){ dispose(); }
    
    void dispose(){
        _object = nullptr;
    }

	void update(std::shared_ptr<GameState> state) { }

	bool garbageCollect(GameObject* obj) { return true; }

	std::vector<std::shared_ptr<GameObject>> getObjects() {
		std::vector<std::shared_ptr<GameObject>> objects;
		objects.push_back(_object);
		return objects;
	}

	bool isActive() {return _isActive;}

	void toggleActive() { _isActive = !_isActive;  }

	bool init(std::shared_ptr<GameObject> object) {
		_object = object;
		_isActive = false;
		return true;
	}

	static std::shared_ptr<StaticAI> alloc(std::shared_ptr<GameObject> object) {
		std::shared_ptr<StaticAI> result = std::make_shared<StaticAI>();
		return (result->init(object) ? result : nullptr);
	}

	std::shared_ptr<cugl::JsonValue> StaticAI::toJsonValue() {
		std::shared_ptr<cugl::JsonValue> fullJson = cugl::JsonValue::allocObject();
		fullJson->appendChild("aiType", cugl::JsonValue::alloc("STATIC"));
		fullJson->appendChild("uid", cugl::JsonValue::alloc(std::to_string(_object->getUid())));
		return fullJson;
	}

	static std::shared_ptr<StaticAI> allocWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state)
	{
		std::shared_ptr<StaticAI> result = std::make_shared<StaticAI>();
		return (result->initWithJson(json, state) ? result : nullptr);
	}

	bool initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state) {
		std::shared_ptr<GameObject> gObj = state->getUID2GameObject(std::stoi(json->getString("uid")));
		init(gObj);
		return true;
	}
};

#endif /* StaticAI_hpp */
