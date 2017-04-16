//
//  WaveData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef WaveData_hpp
#define WaveData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "AIData.hpp"

class WaveEntry {
private:
    std::string objectKey;
    
    std::string aiKey;
    
    Element element;
    
    cugl::Vec2 position;

    std::vector<std::string> zoneKeys;
    
    std::string templateKey;
    
public:
    
    WaveEntry(){}
    
    std::string getObjectKey();
    
    std::string getAIKey();
    
    Element getElement();
    
    cugl::Vec2 getPosition();
    
    std::vector<std::string> getZoneKeys();
    
    std::string getTemplateKey();
    
    void setTemplateKey(std::string tKey);
    
    void setPosition(cugl::Vec2 pos);
    
    void switchElement(std::string blueObjectKey, std::string goldObjectKey) {
        element = element == Element::BLUE ? Element::GOLD : Element::BLUE;
        objectKey = element == Element::BLUE ? blueObjectKey : goldObjectKey;
    }
    
    bool init(const std::shared_ptr<cugl::JsonValue>& json);
    
    bool init(std::string objectKey, std::string aiKey, float x, float y,Element element,std::vector<std::string> zoneKeys);
    
    static std::shared_ptr<WaveEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json){
        std::shared_ptr<WaveEntry> result = std::make_shared<WaveEntry>();
        return (result->init(json) ? result : nullptr);
    }
    
    static std::shared_ptr<WaveEntry> alloc(std::string objectKey, std::string aiKey, float x, float y, Element element, std::vector<std::string> zoneKeys) {
        std::shared_ptr<WaveEntry> result = std::make_shared<WaveEntry>();
        return (result->init(objectKey, aiKey,x,y, element,zoneKeys) ? result : nullptr);
    }

	std::shared_ptr<cugl::JsonValue> toJsonValue();
};

class WaveData : public Data {
protected:
    std::vector<std::shared_ptr<WaveEntry>> _waveEntries;
    
public:
    WaveData() : Data(){}
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<WaveData> alloc() {
        std::shared_ptr<WaveData> result = std::make_shared<WaveData>();
        return (result->init() ? result : nullptr);
    }
    
    void addWaveEntry(std::shared_ptr<WaveEntry> w){
        _waveEntries.push_back(w);
    }
    
    std::vector<std::shared_ptr<WaveEntry>> getWaveEntries(){
        return _waveEntries;
    }

	std::shared_ptr<WaveEntry> getEntry(int index) {
		return _waveEntries.at(index);
	}
    
    
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* WaveData_hpp */
