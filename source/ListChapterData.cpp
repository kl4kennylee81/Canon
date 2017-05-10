#include "ListChapterData.hpp"
using namespace cugl;

bool ListChapterData::preload(const std::string& file)
{
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool ListChapterData::preload(const std::shared_ptr<cugl::JsonValue>& json)
{
	init(json->getString("name"),
		json->get("levels")->asStringArray());

	return true;
}