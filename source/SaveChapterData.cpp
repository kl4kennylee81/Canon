#include "SaveChapterData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> SaveChapterData::toJsonValue()
{
	std::shared_ptr<JsonValue> obj = JsonValue::allocObject();
	obj->appendChild("unlocked", JsonValue::alloc(unlocked ? 1. : 0.));
	obj->appendChild("complete", JsonValue::alloc(complete ? 1. : 0.));
	return obj;
}


bool SaveChapterData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool SaveChapterData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	Data::preload(json);
	init(json->getBool("complete"),
		json->getBool("unlocked"));

	return true;
}