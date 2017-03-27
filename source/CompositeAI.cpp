
#include "CompositeAI.hpp"


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