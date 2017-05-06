#include "MenuListData.hpp"
using namespace cugl;

std::shared_ptr<JsonValue> MenuListData::toJsonValue()
{
	return JsonValue::allocNull();
}

bool MenuListData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool MenuListData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	std::string smKey = json->getString("startMenuKey");
	std::vector<std::string> entries = json->get("menuEntries")->asStringArray();
	init(smKey, entries);
	return true;
}

bool MenuListData::materialize() {
	return true;
}