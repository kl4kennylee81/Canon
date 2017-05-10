#include "SaveLevelData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> SaveLevelData::toJsonValue()
{

	std::shared_ptr<JsonValue> level = JsonValue::allocObject();
	level->appendChild("unlocked", JsonValue::alloc(unlocked ? 1. : 0.));
	level->appendChild("complete", JsonValue::alloc(complete ? 1. : 0.));
	level->appendChild("highScore", JsonValue::alloc((float)highScore));
	
	return level;
}


bool SaveLevelData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool SaveLevelData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	Data::preload(json);
	init(json->getBool("complete"),
		json->getBool("unlocked"),
		json->getFloat("highScore"));

	return true;
}