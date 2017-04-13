#include "UIData.hpp"

using namespace cugl;


bool ButtonUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	string type = json->get("buttonAction")->getString("type");
	buttonAction = ButtonAction::alloc(type);
	if (type == "uiChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		UIChangeButtonAction::alloc(type, json->get("buttonAction")->getString("buttonTarget"))); }
	else if (type == "menuChange") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		MenuChangeButtonAction::alloc(type, json->get("buttonAction")->getString("buttonTarget"))); }
	else if (type == "fxTrigger") { buttonAction = std::dynamic_pointer_cast<ButtonAction>(
		FxTriggerButtonAction::alloc(type, json->getString("fxKey"))); }

	buttonLabel = json->getString("buttonLabel");
	UIData::preload(json); // call to super
	return true;
}

bool TextUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	textValue = json->getString("textValue");
	fontKey = json->getString("fontKey");
	UIData::preload(json);
	return true;
}

bool ImageUIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	textureKey = json->getString("textureKey");
	UIData::preload(json);
	return true;
}


std::shared_ptr<JsonValue> UIData::toJsonValue()
{
	return JsonValue::allocNull();
}

std::string UIData::serialize() {
	return toJsonValue()->toString();
}

bool UIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool UIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	UIKey = json->key();
	x = json->getInt("x");
	y = json->getInt("y");
	width = json->getInt("width");
	height = json->getInt("height");
	return true;
}

bool UIData::materialize() {
	return true;
}
