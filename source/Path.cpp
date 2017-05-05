//
//  Path.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Path.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> Path::toJsonValue() {
	std::shared_ptr<JsonValue> obj = JsonValue::allocArray();
	for (int i = 0; i < _coordinates.size(); i++) {
		std::shared_ptr<JsonValue> pair = JsonValue::allocObject();
		pair->appendChild("x", JsonValue::alloc(_coordinates.at(i).x));
		pair->appendChild("y", JsonValue::alloc(_coordinates.at(i).y));
		obj->appendChild(pair);
	}
	return obj;
}