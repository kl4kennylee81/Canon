#include "CompositeAIData.hpp"
#include "CompositeAI.hpp"
#include "Util.hpp"

using namespace cugl;

std::shared_ptr<JsonValue> CompositeAIData::toJsonValue()
{
	std::shared_ptr<JsonValue> data = JsonValue::allocObject();
	data->appendChild("type", JsonValue::alloc("COMPOSITE"));
	data->appendChild("startAI", JsonValue::alloc(_startKey));
	std::shared_ptr<JsonValue> conditions = JsonValue::allocArray();

	for (int i = 0; i < getConditions().size(); i++)
	{
		std::shared_ptr<JsonValue> cond = JsonValue::allocObject();
		cond->appendChild("condition", JsonValue::alloc(getConditions().at(i)->getJsonString()));
		cond->appendChild("aiKey", JsonValue::alloc(_aiKeys.at(i)));
		conditions->appendChild(cond);
	}

	data->appendChild("conditions", conditions);
	return data;
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
	std::string startKey = json->getString("startAI");
	init(startKey, conditions, aiKeys);
	return true;
}

bool CompositeAIData::materialize() {
	return true;
}

std::shared_ptr<ActiveAI> CompositeAIData::newActiveAI(std::shared_ptr<GameObject> object) {
	return CompositeAI::alloc(object, _startData, _conditions, _aiDatas);
}