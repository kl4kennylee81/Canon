#include "ChapterSelectData.hpp"
using namespace cugl;

bool ChapterSelectData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool ChapterSelectData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	Data::preload(json);
	init(json->getString("name"),
		json->get("levels")->asStringArray(), json->getString("chapterTexture"));
    continuous = json->getBool("continuous");

	return true;
}
