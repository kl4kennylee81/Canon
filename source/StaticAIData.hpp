//
//  StaticZoneData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef StaticAIData_hpp
#define StaticAIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AIData.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class StaticAIData : public AIData {
public:

	StaticAIData() : AIData() {}

	bool init() { return true; }

	static std::shared_ptr<StaticAIData> alloc() {
		std::shared_ptr<StaticAIData> result = std::make_shared<StaticAIData>();
		return (result->init() ? result : nullptr);
	}

	std::string serialize() override;

	bool preload(const std::string& file) override;

	bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool materialize() override;

	std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;
};

#endif /* StaticAIData.hpp */