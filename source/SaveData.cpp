#include "SaveData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> SaveData::toJsonValue()
{
	std::shared_ptr<JsonValue> level = JsonValue::allocObject();
	level->appendChild("currentChapter", JsonValue::alloc(currentChapter));
	level->appendChild("volumeOn", JsonValue::alloc(volumeOn ? 1. : 0.));
	level->appendChild("soundFXOn", JsonValue::alloc(soundFXOn ? 1. : 0.));
	return level;
}


bool SaveData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool SaveData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	init(json->getString("currentChapter"),
		json->getBool("volumeOn"),
		json->getBool("volumeOff"));
	return true;
}