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
	HOMING, PATH, STATIC, COMPOSITE, BULLET
};

class AIData : public Data {
public:

	AIType type;

	AIData() : Data() {}

	virtual bool preload(const std::string& file) { return true;  }

	virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) {
		Data::preload(json); 
		return true;  
	}

	virtual bool materialize() { return true;  }

	virtual std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) { return nullptr;  }

	static AIType stringToAIType(std::string at) {
		//HOMING, PATH, STATIC, COMPOSITE
		if (at == "HOMING") return AIType::HOMING;
		if (at == "PATH") return AIType::PATH;
		if (at == "STATIC") return AIType::STATIC;
		if (at == "COMPOSITE") return AIType::COMPOSITE;
        if (at == "BULLET") return AIType::BULLET;
		return AIType::STATIC;
	}

};
#endif /* AIData_hpp */
