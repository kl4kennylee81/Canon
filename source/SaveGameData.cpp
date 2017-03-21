#include "SaveGameData.h"
#include "LevelData.hpp"

using namespace cugl;

std::string SaveGameData::serialize() 
{
	return "";
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
			auto entry = MenuEntry::alloc(
				child->getString("menuEntry"), 
				child->getBool("unlocked"), 
				child->getBool("complete"), 
				child->getInt("highScore"));
			_menuEntries.push_back(entry);

			// todo: have asset manager use the level key to load in that specific level
			auto levelKey = child->get("levelKey");
		}
	}
	return true;
}

bool SaveGameData::materialize() 
{
	return true;
}
