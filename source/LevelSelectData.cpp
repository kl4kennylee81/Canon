#include "LevelSelectData.hpp"
using namespace cugl;

bool LevelSelectData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool LevelSelectData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	Data::preload(json);
	init(json->getString("name"),
		json->getString("levelKey"));
	return true;
}
