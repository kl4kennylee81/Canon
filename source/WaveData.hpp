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
    
    WaveEntry(int objectKey, float x, float y){
        objectKey = objectKey;
        position = cugl::Vec2::Vec2(x,y);
    }
};

class WaveData : public Data {
protected:
    std::vector<WaveEntry> _waveEntries;
    
public:
    WaveData() : Data(){}
    
    WaveData(int uid) : Data(uid){
        
    }
    
    bool init(int uid) {
        uid = uid;
        return true;
    }
    
    static std::shared_ptr<WaveData> alloc(int uid) {
        std::shared_ptr<WaveData> result = std::make_shared<WaveData>();
        return (result->init(uid) ? result : nullptr);
    }
    
    void addWaveEntry(WaveEntry w){
        _waveEntries.push_back(w);
    }
    
    WaveEntry getWaveEntry(int index){
        return _waveEntries.at(index);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* WaveData_hpp */
