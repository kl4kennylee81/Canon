#include "HomingAIData.hpp"
#include "HomingAI.hpp"


using namespace cugl;

std::shared_ptr<JsonValue> HomingAIData::toJsonValue() {
	std::shared_ptr<JsonValue> ai = JsonValue::allocObject();
	ai->appendChild("type", JsonValue::alloc("HOMING"));
	return ai;
}

bool HomingAIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool HomingAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::HOMING;
	init();
	Data::preload(json);
	return true;
}

bool HomingAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> HomingAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return HomingAI::alloc(object);
}