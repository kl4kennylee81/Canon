//
//  Data.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Data.hpp"

using namespace cugl;

bool Data::preload(const std::string& file) {
    Asset::preload(file);
    auto reader = JsonReader::allocWithAsset(file.c_str());
    auto json = reader->readJson();
    return preload(json);
}

bool Data::preload(const std::shared_ptr<cugl::JsonValue>& json) {
    Asset::preload(json);
	key = json->getString("key");
    return true;
}

std::shared_ptr<JsonValue> Data::toJsonValue() {
	std::shared_ptr<JsonValue> jv = JsonValue::allocObject();
	jv->appendChild("key", JsonValue::alloc(key));
	return jv;
}

bool Data::materialize(){
    Asset::materialize();
    return true;
}
