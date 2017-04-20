//
//  HomingAI.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef CompositeAI_hpp
#define CompositeAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"
#include "Condition.hpp"
#include "AIData.hpp"

class CompositeAI : public ActiveAI {
private:
	bool _isActive;
	std::shared_ptr<GameObject> _object;
	std::shared_ptr<ActiveAI> _activeAI;
	std::vector<std::shared_ptr<Condition>> _conditions;
	std::vector<std::shared_ptr<AIData>> _datas;
	int _index;

public:
    
    CompositeAI(){}
    
    ~CompositeAI(){
        _object = nullptr;
        _activeAI = nullptr;
    }

	void update(std::shared_ptr<GameState> state);

	bool garbageCollect(GameObject* obj);

	std::vector<std::shared_ptr<GameObject>> getObjects() {
		std::vector<std::shared_ptr<GameObject>> objects;
		objects.push_back(_object);
		return objects;
	}

	bool isActive() { return _isActive; }

	void toggleActive() { _isActive = !_isActive; }

	bool init(std::shared_ptr<GameObject> object, std::shared_ptr<AIData> startData, 
		std::vector<std::shared_ptr<Condition>> conditions, std::vector<std::shared_ptr<AIData>> datas)
	{
		_object = object;
		_isActive = false;
		_conditions = conditions;
		_datas = datas;
		_activeAI = startData->newActiveAI(object);
		_index = 0;
		return true;
	}

	static std::shared_ptr<CompositeAI> alloc(std::shared_ptr<GameObject> object, std::shared_ptr<AIData> startData,
		std::vector<std::shared_ptr<Condition>> conditions, std::vector<std::shared_ptr<AIData>> datas)
	{
		std::shared_ptr<CompositeAI> result = std::make_shared<CompositeAI>();
		return (result->init(object, startData, conditions, datas) ? result : nullptr);
	}
};

#endif /* CompositeAI_hpp */
