//
//  InputController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef InputController_hpp
#define InputController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameEngine.hpp"

using namespace cugl;

class InputController {
public:
	static bool _touch;

	static cugl::Vec2 getInputVector() {
		if (GameEngine::_touch) {
			auto set = Input::get<Touchscreen>()->touchSet();
			return set.size() > 0 ? Input::get<Touchscreen>()->touchPosition(set.at(0)) : Vec2::Vec2();
		}
		else {
			return Input::get<Mouse>()->pointerPosition();
		}
	}

	static bool getIsPressed() {
		if (GameEngine::_touch) {
			return Input::get<Touchscreen>()->touchSet().size() > 0;
		}
		else {
			return Input::get<Mouse>()->buttonDown().hasLeft();
		}
	}

	static bool getDoubleTouch() {
		if (GameEngine::_touch) {
			return Input::get<Touchscreen>()->touchSet().size() > 1;
		}
		else {
			return Input::get<Mouse>()->buttonDown().hasLeft() &&
				Input::get<Mouse>()->buttonDown().hasRight();
		}
	}
};

#endif /* InputController_hpp */
