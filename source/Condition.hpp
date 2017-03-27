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

#endif /* Condition_hpp */
