//
//  LevelData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelData_hpp
#define LevelData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "WaveData.hpp"
#include <vector>

class LevelEntry : public Data {
public:
    std::string waveKey;
    float time; // frames
    
    LevelEntry():Data(){}
    
    bool init(){
        Data::init();
        this->waveKey = "";
        this->time = 0;
        return true;
    }
    
    bool init(std::string wkey,int t){
        init();
        this->waveKey = wkey;
        this->time = t;
        return true;
    }
    
    static std::shared_ptr<LevelEntry> alloc() {
        std::shared_ptr<LevelEntry> result = std::make_shared<LevelEntry>();
        return (result->init() ? result : nullptr);
    }

    static std::shared_ptr<LevelEntry> alloc(std::string waveKey,int time) {
        std::shared_ptr<LevelEntry> result = std::make_shared<LevelEntry>();
        return (result->init(waveKey,time) ? result : nullptr);
    }
    
    static std::shared_ptr<LevelEntry> alloc(const std::shared_ptr<cugl::JsonValue>& json) {
        std::shared_ptr<LevelEntry> result = std::make_shared<LevelEntry>();
        return (result->preload(json) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file) override;
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual bool materialize() override;
};

class LevelData : public Data {
protected:
    
    // wave entries for the initial position of the player characters
    std::vector<std::shared_ptr<WaveEntry>> _playerChars;
    
    std::vector<std::shared_ptr<LevelEntry>> _levelEntries;
public:
    LevelData() : Data(){}
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<LevelData> alloc() {
        std::shared_ptr<LevelData> result = std::make_shared<LevelData>();
        return (result->init() ? result : nullptr);
    }
    
    void addLevelEntry(std::shared_ptr<LevelEntry> entry);
    
    void updateEntryTime(int index, float time);
    
    void addPlayerChars(std::shared_ptr<WaveEntry> entry);
    
    std::vector<std::shared_ptr<WaveEntry>>& getPlayerChars() {
        return _playerChars;
    }
    
    void removeWave(int index){
        std::vector<std::shared_ptr<LevelEntry>> newEntries;
        for(int i = 0; i < _levelEntries.size(); i++){
            if(i != index){
                newEntries.push_back(_levelEntries.at(i));
            }
        }
        _levelEntries = newEntries;
    }
    
    float getTime(int index);
    
    std::string getWaveKey(int index);
    
    void setWaveKey(int index, std::string newKey){
        _levelEntries.at(index)->waveKey = newKey;
    }
    
    size_t getNumberWaves();

	std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file) override;
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual bool materialize() override;
    
    bool isValid();
};

#endif /* LevelData_hpp */
