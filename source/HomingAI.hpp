//
//  HomingAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef HomingAI_hpp
#define HomingAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class HomingAI : public ActiveAI {
public:
	std::shared_ptr<GameObject> _object;

	void update(std::shared_ptr<GameState> state);

	bool garbageCollect(GameObject* obj);

	std::vector<std::shared_ptr<GameObject>> getObjects();

	bool init(std::shared_ptr<GameObject> object) {
		_object = object;
		return true;
	}

	static std::shared_ptr<HomingAI> alloc(std::shared_ptr<GameObject> object) {
		std::shared_ptr<HomingAI> result = std::make_shared<HomingAI>();
		return (result->init(object) ? result : nullptr);
	}
};

#endif /* HomingAI_hpp */
