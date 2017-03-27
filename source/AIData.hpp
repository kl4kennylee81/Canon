//
//  WaveData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AIData_hpp
#define AIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"
#include "StaticAI.hpp"

enum class AIType : int {
	HOMING, PATH, STATIC, COMPOSITE
};

class AIData : public Data {
public:

	AIType type;

	AIData() : Data() {}

	virtual std::string serialize() { return "";  };

	virtual bool preload(const std::string& file) { return true;  }

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) { return true;  }

	virtual bool materialize() { return true;  }

	virtual std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) { return nullptr;  }

};
#endif /* AIData_hpp */
