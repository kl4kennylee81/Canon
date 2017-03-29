#include "StaticAIData.hpp"
#include "StaticAI.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> StaticAIData::toJsonValue() {
	std::shared_ptr<JsonValue> ai = JsonValue::allocObject();
	ai->appendChild("type", JsonValue::alloc("STATIC"));
	return ai;
}

bool StaticAIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool StaticAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::STATIC;
	init();

	return true;
}

bool StaticAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> StaticAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return StaticAI::alloc(object);
}