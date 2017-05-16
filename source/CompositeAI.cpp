
#include "CompositeAI.hpp"

using namespace cugl;

void CompositeAI::update(std::shared_ptr<GameState> state) {
	if (_index < _conditions.size()) {
		auto condition = _conditions.at(_index);
		if (condition->checkCondition(state, _object)) {
			_activeAI = _datas.at(_index)->newActiveAI(_object);
			_index = _index + 1;
		}
	}
	_activeAI->update(state);
}

bool CompositeAI::garbageCollect(GameObject* obj) { 
	return _activeAI->garbageCollect(obj);
}

std::shared_ptr<JsonValue> CompositeAI::toJsonValue() {
	std::shared_ptr<JsonValue> fullJson = JsonValue::allocObject();

	//std::shared_ptr<GameObject> _object;

	//std::shared_ptr<ActiveAI> _activeAI;

	//std::vector<std::shared_ptr<Condition>> _conditions;
	//std::vector<std::shared_ptr<AIData>> _datas;
	//int _index;

	fullJson->appendChild("aiType", cugl::JsonValue::alloc("COMPOSITE"));
	fullJson->appendChild("uid", cugl::JsonValue::alloc(std::to_string(_object->getUid())));
	fullJson->appendChild("activeAI", _activeAI->toJsonValue());

	std::shared_ptr<JsonValue> conditions = JsonValue::allocObject();
	for (auto entry : _conditions) {
		conditions->appendChild(JsonValue::alloc(entry->getJsonString()));
	}

	std::shared_ptr<JsonValue> datas = JsonValue::allocObject();
	for (auto entry : _datas) {
		datas->appendChild(entry->toJsonValue());
	}

	fullJson->appendChild("conditions", conditions);
	fullJson->appendChild("datas", datas);

	return fullJson;
}

bool CompositeAI::initWithJson(std::shared_ptr<JsonValue> json, std::shared_ptr<GameState> state) {
	// TODO once we have composite AI
	return true;
}