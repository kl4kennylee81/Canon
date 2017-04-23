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
    ElementType element;
    
    cugl::Vec2 position; //physics coordinates
    
    std::string templateKey;
    
    std::string aiKey;
    
public:
    
    WaveEntry(){}
    
    ElementType getElement();
    
    cugl::Vec2 getPosition();
    
    std::string getAIKey() {
        return aiKey;
    }
    
    std::string getTemplateKey();
    
    void setTemplateKey(std::string tKey);
    
    void setPosition(cugl::Vec2 pos);
    
    void setAIKey(std::string aKey) { aiKey = aKey; }
    
    void switchElement() {
        element = element == ElementType::BLUE ? ElementType::GOLD : ElementType::BLUE;
    }
    
    bool init(const std::shared_ptr<cugl::JsonValue>& json);
    
    bool init(float x, float y,ElementType element,std::string templateKey, std::string aiKey);
    
    static std::shared_ptr<WaveEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json){
        std::shared_ptr<WaveEntry> result = std::make_shared<WaveEntry>();
        return (result->init(json) ? result : nullptr);
    }

    static std::shared_ptr<WaveEntry> alloc(float x, float y,ElementType element,std::string templateKey, std::string aiKey) {
        std::shared_ptr<WaveEntry> result = std::make_shared<WaveEntry>();
        return (result->init(x,y,element,templateKey, aiKey) ? result : nullptr);
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
    
    bool init(std::shared_ptr<WaveData> data) {
        for(auto it: data->getWaveEntries()){
            auto we = WaveEntry::alloc(0, 0, it->getElement(), it->getTemplateKey(), it->getAIKey());
            we->setPosition(it->getPosition()); //already in physics coordinates
            _waveEntries.push_back(we);
        }
        return true;
    }
    
    static std::shared_ptr<WaveData> alloc() {
        std::shared_ptr<WaveData> result = std::make_shared<WaveData>();
        return (result->init() ? result : nullptr);
    }
    
    static std::shared_ptr<WaveData> alloc(std::shared_ptr<WaveData> data) {
        std::shared_ptr<WaveData> result = std::make_shared<WaveData>();
        return (result->init(data) ? result : nullptr);
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
    
    void removeEntry(int index) {
        std::vector<std::shared_ptr<WaveEntry>> newEntries;
        for(int i = 0; i < _waveEntries.size(); i++) {
            if(i != index){
                newEntries.push_back(_waveEntries.at(i));
            }
        }
        _waveEntries = newEntries;
    }
    
    
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* WaveData_hpp */
