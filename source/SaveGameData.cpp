#include "SaveGameData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> SaveGameData::toJsonValue()
{
	std::shared_ptr<JsonValue> levelParent = JsonValue::allocObject();
	for (int i = 0; i < _saveLevelEntries.size(); i++)
	{
		auto child = _saveLevelEntries.at(i);
		std::shared_ptr<JsonValue> level = JsonValue::allocObject();
		level->appendChild("name", JsonValue::alloc(child->name));
		level->appendChild("unlocked", JsonValue::alloc((child->unlocked) ? 1. : 0.));
		level->appendChild("complete", JsonValue::alloc((child->complete) ? 1. : 0.));
		level->appendChild("levelKey", JsonValue::alloc(child->levelKey));
		level->appendChild("levelUrl", JsonValue::alloc(child->levelUrl));
		level->appendChild("highScore", JsonValue::alloc(static_cast<double>(child->highScore)));
		levelParent->appendChild(child->levelKey, level);
		
	}
	return levelParent;
}


bool SaveGameData::preload(const std::string& file) 
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool SaveGameData::preload(const std::shared_ptr<cugl::JsonValue>& json) 
{
	if (json->type() == JsonValue::Type::ObjectType) 
	{
		for (int i = 0; i < json->size(); i++)
		{
			auto child = json->get(i);
			auto entry = SaveLevelEntry::alloc(
				child->getString("name"), 
				child->getBool("unlocked"), 
				child->getBool("complete"), 
				child->getInt("highScore"),
				child->getString("levelKey"),
				child->getString("levelUrl"));
			_saveLevelEntries.push_back(entry);

			// todo: have asset manager use the level key to load in that specific level
			auto levelKey = child->get("levelKey");
		}
	}

	return true;
}

std::vector<std::shared_ptr<SaveLevelEntry>> SaveGameData::getSaveLevelEntries()
{
    return _saveLevelEntries;
}

bool SaveGameData::materialize()
{
	return true;
}
