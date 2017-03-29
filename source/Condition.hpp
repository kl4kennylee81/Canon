//
//  ActiveAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Condition_hpp
#define Condition_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "GameObject.hpp"
#include "Element.hpp"

class Condition {
public:
	Condition() {}

	virtual bool checkCondition(std::shared_ptr<GameState> state, std::shared_ptr<GameObject> object) { return true; }

	bool init() { return true; }

	static std::shared_ptr<Condition> alloc() {
		std::shared_ptr<Condition> result = std::make_shared<Condition>();
		return (result->init() ? result : nullptr);
	}
};

class RadiusCondition : public Condition {
public:
	float _radius; // in physics coordinates

	RadiusCondition() : Condition() {}

	virtual bool checkCondition(std::shared_ptr<GameState> state, std::shared_ptr<GameObject> object) { 
		Element myElement = object->getPhysicsComponent()->getElementType();
		auto players = state->getPlayerCharacters();
		for (auto it : players) {
			if (it->getPhysicsComponent()->getElementType() != myElement) {
				cugl::Vec2 myPos = object->getPosition();
				cugl::Vec2 enemyPos = it->getPosition();
				if (myPos.distance(enemyPos) < _radius) {
					return true;
				}
			}
		}
		return false; 
	}

	bool init(std::string radius) { 
		_radius = std::stof(radius) / GAME_PHYSICS_SCALE;
		return true; 
	}

	static std::shared_ptr<RadiusCondition> alloc(std::string radius) {
		std::shared_ptr<RadiusCondition> result = std::make_shared<RadiusCondition>();
		return (result->init(radius) ? result : nullptr);
	}
};

#endif /* Condition_hpp */
