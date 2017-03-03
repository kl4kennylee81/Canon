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

class WaveEntry {
public:
    int objectKey;
    
    cugl::Vec2 position;
    
    WaveEntry(){}
    
    bool init(int oKey, float x, float y){
        this->objectKey = oKey;
        this->position.x = x;
        this->position.y = y;
        return true;
    }
    
    static std::shared_ptr<WaveEntry> alloc(int objectKey, float x, float y) {
        std::shared_ptr<WaveEntry> result = std::make_shared<WaveEntry>();
        return (result->init(objectKey,x,y) ? result : nullptr);
    }
};

class WaveData : public Data {
protected:
    std::vector<std::shared_ptr<WaveEntry>> _waveEntries;
    
public:
    WaveData() : Data(){}
    
    bool init(int uid) {
        this->_uid = uid;
        return true;
    }
    
    static std::shared_ptr<WaveData> alloc(int uid) {
        std::shared_ptr<WaveData> result = std::make_shared<WaveData>();
        return (result->init(uid) ? result : nullptr);
    }
    
    void addWaveEntry(std::shared_ptr<WaveEntry> w){
        _waveEntries.push_back(w);
    }
    
    std::vector<std::shared_ptr<WaveEntry>> getWaveEntries(){
        return _waveEntries;
    }
    
    
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* WaveData_hpp */
