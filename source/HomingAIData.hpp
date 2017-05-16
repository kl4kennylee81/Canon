//
//  PathAIData.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef HomingAIData_hpp
#define HomingAIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AIData.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class HomingAIData : public AIData {
public:

	HomingAIData() : AIData() {}

	bool init() { return true; }

	static std::shared_ptr<HomingAIData> alloc() {
		std::shared_ptr<HomingAIData> result = std::make_shared<HomingAIData>();
		return (result->init() ? result : nullptr);
	}

	std::shared_ptr<cugl::JsonValue> toJsonValue() override;

	bool preload(const std::string& file) override;

	bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

	bool materialize() override;

	std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;
};

#endif /* HomingAIData.hpp */