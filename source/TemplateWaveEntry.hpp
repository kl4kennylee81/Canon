//
//  WaveData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TemplateWaveEntry_hpp
#define TemplateWaveEntry_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "AIData.hpp"

class TemplateWaveEntry {
public:
	std::string name;

	std::string objectKey;

	std::string aiKey;

	std::vector<std::string> zoneKeys;

	TemplateWaveEntry() {}

	/* update the data file which corresponds to this template*/
	bool updateFile();

	bool init(const std::shared_ptr<cugl::JsonValue>& json);

	static std::shared_ptr<TemplateWaveEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(json) ? result : nullptr);
	}

	bool init(std::string name, std::string objectKey, std::string aiKey, std::vector<std::string> zoneKeys);

	static std::shared_ptr<TemplateWaveEntry> alloc(std::string name, std::string objectKey, std::string aiKey,
		std::vector<std::string> zoneKeys) 
	{
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(name, objectKey, aiKey, zoneKeys) ? result : nullptr);
	}
};
#endif /* TemplateWaveData_hpp */
