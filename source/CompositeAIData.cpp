#include "CompositeAIData.hpp"
#include "CompositeAI.hpp"
#include "Util.hpp"

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

std::shared_ptr<Condition> getConditionFromString(const std::shared_ptr<cugl::JsonValue>& child) {
	std::vector<std::string> vec = Util::split(child->getString("condition"), ' ');
	if (vec.at(0).compare("RADIUS") == 0) {
		return RadiusCondition::alloc(vec.at(1));
	}
	return Condition::alloc();
}

bool CompositeAIData::preload(const std::shared_ptr<cugl::JsonValue>& json) {
	type = AIType::COMPOSITE;
	std::vector<std::shared_ptr<Condition>> conditions;
	std::vector<std::string> aiKeys;
	auto conds = json->get("conditions");
	for (int i = 0; i < conds->size(); i++) {
		std::shared_ptr<Condition> condition = getConditionFromString(conds->get(i));
		conditions.push_back(condition);
		aiKeys.push_back(conds->get(i)->getString("aiKey"));
	}
	init(conditions, aiKeys);
	return true;
}

bool CompositeAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> CompositeAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return CompositeAI::alloc(object, _conditions, _aiDatas);
}