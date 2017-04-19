//
//  WaveData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TemplateWaveEntry_hpp
#define TemplateWaveEntry_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "AIData.hpp"

class TemplateWaveEntry : public Data {
private:
    std::string objectKey;
    
    float _spawnTime;
    
public:
    std::string name;
    
    std::string aiKey;
    
    std::vector<std::string> zoneKeys;
    
    TemplateWaveEntry() : Data(){}

	/* update the data file which corresponds to this template*/
	bool updateFile();

	bool init(const std::shared_ptr<cugl::JsonValue>& json);

	static std::shared_ptr<TemplateWaveEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(json) ? result : nullptr);
	}

    bool init(std::string name, std::string obKey, std::string aiKey, std::vector<std::string> zoneKeys,float spawnTime);
    
    static std::shared_ptr<TemplateWaveEntry> alloc(std::string name, std::string obKey,
                                                    std::string aiKey, std::vector<std::string> zoneKeys,float spawnTime)
	{
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(name, obKey, aiKey, zoneKeys,spawnTime) ? result : nullptr);
	}
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    std::string getName();
    
    std::string getObjectKey();
    
    std::string getAIKey();
    
    std::vector<std::string> getZoneKeys();
    
    float getSpawnTime();
};
#endif /* TemplateWaveData_hpp */
