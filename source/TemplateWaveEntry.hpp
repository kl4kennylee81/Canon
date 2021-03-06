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
    
    std::vector<std::string> aiKeys;
    
    std::vector<std::string> zoneKeys;
    
    std::vector<std::vector<std::string>> allZoneKeys;
    
    std::string bulletKey;
    
    TemplateWaveEntry() : Data(){}

	/* update the data file which corresponds to this template*/
	bool updateFile();

	bool init(const std::shared_ptr<cugl::JsonValue>& json);

	static std::shared_ptr<TemplateWaveEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(json) ? result : nullptr);
	}


    bool init(std::string name, std::string obKey, std::vector<std::string> aiKeys,
        std::vector<std::string> zoneKeys,float spawnTime, std::string bullet);
    
    static std::shared_ptr<TemplateWaveEntry> alloc(std::string name, std::string obKey, std::vector<std::string> aiKeys,
        std::vector<std::string> zoneKeys, float spawnTime, std::string bullet)
	{
		std::shared_ptr<TemplateWaveEntry> result = std::make_shared<TemplateWaveEntry>();
		return (result->init(name, obKey, aiKeys, zoneKeys,spawnTime, bullet) ? result : nullptr);
	}
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    std::string getName();
    
    std::string getObjectKey();
    
    std::string getAIKey(int index) { return aiKeys.at(index); }
    
    std::string getNextAIKey(std::string currentKey);
    
    std::vector<std::string> getNextZoneKeys(std::vector<std::string> currentKeys);
    
    std::vector<std::string> getZoneKeys();
    
    void addZoneKeys(std::vector<std::string> zk){
        allZoneKeys.push_back(zk);
    }
    
    float getSpawnTime();
};
#endif /* TemplateWaveData_hpp */
