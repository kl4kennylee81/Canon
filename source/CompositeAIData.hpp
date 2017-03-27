//
//  StaticZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef CompositeAIData_hpp
#define CompositeAIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AIData.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"
#include "Condition.hpp"

class CompositeAIData : public AIData {
protected:
	std::vector<std::shared_ptr<Condition>> _conditions;

public:
	std::vector<std::string> _aiKeys;
	std::vector<std::shared_ptr<AIData>> _aiDatas;

	CompositeAIData() : AIData() {}

	bool init(std::vector<std::shared_ptr<Condition>> conditions, std::vector<std::string> aiKeys) {
		_conditions = conditions;
		_aiKeys = aiKeys;
		return true;
	}

	static std::shared_ptr<CompositeAIData> alloc(std::vector<std::shared_ptr<Condition>> conditions, std::vector<std::string> aiKeys) {
		std::shared_ptr<CompositeAIData> result = std::make_shared<CompositeAIData>();
		return (result->init(conditions, aiKeys) ? result : nullptr);
	}

	std::string serialize() override;

	bool preload(const std::string& file) override;

	bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool materialize() override;

	std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;
};

#endif /* CompositeAIData_hpp */