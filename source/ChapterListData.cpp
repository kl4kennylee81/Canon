#include "ChapterListData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> ChapterListData::toJsonValue()
{
	return JsonValue::allocNull();
}

bool ChapterListData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool ChapterListData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	std::vector<std::string> entries = json->get("chapterEntries")->asStringArray();
	init(entries);
	return true;
}