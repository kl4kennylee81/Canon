//
//  AnimationData.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationData.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> AnimationUpdate::getJsonValue()
{
	std::shared_ptr<JsonValue> update = JsonValue::allocObject();
	update->appendChild("active", JsonValue::alloc(active));
	update->appendChild("repeat", JsonValue::alloc(repeat));
	return update;
}

std::shared_ptr<JsonValue> AnimationState::getJsonValue()
{
	std::shared_ptr<JsonValue> state = JsonValue::allocObject();
	state->appendChild("first", JsonValue::alloc(static_cast<float>(first)));
	state->appendChild("total", JsonValue::alloc(static_cast<float>(total)));
	std::shared_ptr<JsonValue> frameArray = JsonValue::allocArray();
	for (int i = 0; i < frames.size(); i++)
	{
		frameArray->appendValue(static_cast<float>(frames.at(i)));
	}
	state->appendChild("frames", frameArray);
	return state;
}

std::string AnimationData::serialize(){
	std::shared_ptr<JsonValue> animData = JsonValue::allocObject();
	animData->appendChild("texture", JsonValue::alloc(texture->getName()));
	animData->appendChild("rows", JsonValue::alloc(static_cast<float>(rows)));
	animData->appendChild("cols", JsonValue::alloc(static_cast<float>(cols)));
	animData->appendChild("size", JsonValue::alloc(static_cast<float>(size)));

	std::shared_ptr<JsonValue> stateMap = JsonValue::allocObject();
	std::shared_ptr<JsonValue> actionMap = JsonValue::allocObject();
	
	for (auto const& entry : getStateMap())
	{
		stateMap->appendChild(entry.first, entry.second->getJsonValue());
	}
	for (auto const& entry : getActionMap())
	{
		actionMap->appendChild(AnimationData::actionToString(entry.first), entry.second->getJsonValue());
	}

	animData->appendChild("statemap", stateMap);
	animData->appendChild("actionmap", actionMap);
	return animData->toString();
}

bool AnimationData::preload(const std::string& file){
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
    return true;
}

bool AnimationData::preload(const std::shared_ptr<cugl::JsonValue>& json){
    init();
    texture = Texture::allocWithFile(json->getString("texture"));
	texture->setName(json->getString("texture")); // a way to preserve the texture path
    rows = json->getInt("rows");
    cols = json->getInt("cols");
    size = json->getInt("size");
    auto statemapjson = json->get("statemap");
    for (int i = 0; i < statemapjson->size(); i++) {
        auto statejson = statemapjson->get(i);
        std::string state = statejson->key();
        int first = statejson->getInt("first");
        int total = statejson->getInt("total");
        std::vector<int> frames = statejson->get("frames")->asIntArray();
        auto animationstate = AnimationState::alloc(first,total,frames);
        _statemap.insert({state,animationstate});
    }
    
    auto actionmapjson = json->get("actionmap");
    for (int i = 0; i < actionmapjson->size(); i++) {
        auto actionjson = actionmapjson->get(i);
        
        std::string actionString = actionjson->key();
        auto action = stringToAction(actionString);
        
        std::string active;
        if (actionjson->has("active")){
            active = actionjson->getString("active");
        }
        std::string repeat;
        if (actionjson->has("repeat")){
            repeat = actionjson->getString("repeat");
        }
        auto animationupdate = AnimationUpdate::alloc(active, repeat);
        
        _actionmap.insert({action,animationupdate});
    }
	return true;
}

bool AnimationData::materialize(){
    return true;
}
