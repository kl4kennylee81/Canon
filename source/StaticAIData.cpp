#include "StaticAIData.hpp"
#include "StaticAI.hpp"

using namespace cugl;

std::string StaticAIData::serialize() {
	return "";
}

bool StaticAIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool StaticAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	init();
	return true;
}

bool StaticAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> StaticAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return StaticAI::alloc(object);
}