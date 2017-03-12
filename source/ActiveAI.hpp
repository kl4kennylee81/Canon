//
//  ActivePath.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActiveAI_hpp
#define ActiveAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "GameObject.hpp"

class ActiveAI {
public:
	
	virtual void update(std::shared_ptr<GameState> state) = 0;

	virtual bool garbageCollect(GameObject* obj) = 0;

	virtual std::vector<std::shared_ptr<GameObject>> getObjects() = 0;
};

#endif /* ActivePath_hpp */
