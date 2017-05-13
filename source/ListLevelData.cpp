#include "ListLevelData.hpp"
using namespace cugl;

bool ListLevelData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool ListLevelData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	init(json->getString("name"),
		json->getString("levelKey"));
	return true;
}