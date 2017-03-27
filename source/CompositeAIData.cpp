#include "CompositeAIData.hpp"
#include "CompositeAI.hpp"

using namespace cugl;

std::string CompositeAIData::serialize() {
	return "";
}

bool CompositeAIData::preload(const std::string& file) {
	auto reader = JsonReader::allocWithAsset(file.c_str());
	auto json = reader->readJson();
	preload(json);
	return true;
}

bool CompositeAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::COMPOSITE;
	std::vector<Condition> conditions;
	std::vector<std::string> aiKeys;
	init(conditions, aiKeys);
	return true;
}

bool CompositeAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> CompositeAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return CompositeAI::alloc(object);
}