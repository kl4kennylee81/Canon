#include "SaveGameData.hpp"
//#include <iostream>
//#include <fstream>
using namespace cugl;

std::string SaveGameData::serialize()
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
	return levelParent->toString();
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
	//std::string filename = "test_writer.json";
	//Pathname path = Pathname(filename);
	//std::shared_ptr<JsonWriter> writer = JsonWriter::alloc(path);
	//writer->writeJson(json);

	//std::ofstream myfile;
	//myfile.open("test_writer2.json");
	//myfile << this->serialize();
	//myfile.close();

	return true;
}

bool SaveGameData::materialize() 
{
	return true;
}
