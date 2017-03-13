//
//  AnimationData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AnimationData_hpp
#define AnimationData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "Data.hpp"

enum class AnimationEvent : int {
    SPAWNING,
    SPAWN,
    ACTIVE,
    ATTACK,
    RETURN,
    HIT,
    DEATH,
    DEFAULT
};

class AnimationState {
public:
    int first;
    int total;
    std::vector<int> frames;
    
    bool init(int f, int t, std::vector<int> fs) {
        first = f;
        total = t;
        frames = fs;
        return true;
    }
    
    static std::shared_ptr<AnimationState> alloc(int f, int t, std::vector<int> fs) {
        std::shared_ptr<AnimationState> result = std::make_shared<AnimationState>();
        return (result->init(f,t,fs) ? result : nullptr);
    }
};

class AnimationUpdate {
public:
    
    std::string active;
    std::string repeat;
    
    bool init(std::string a, std::string r) {
        active = a;
        repeat = r;
        return true;
    }
    
    static std::shared_ptr<AnimationUpdate> alloc(std::string active, std::string repeat) {
        std::shared_ptr<AnimationUpdate> result = std::make_shared<AnimationUpdate>();
        return (result->init(active, repeat) ? result : nullptr);
    }
};

class AnimationData : public Data {
protected:
    std::map<std::string, std::shared_ptr<AnimationState>> _statemap;
    
    std::map<AnimationEvent, std::shared_ptr<AnimationUpdate>> _eventmap;
    
public:
    std::shared_ptr<cugl::Texture> texture;
    int rows;
    int cols;
    int size;
    
    AnimationData() : Data(){}
    
    bool init(int uid) {
        this->_uid = uid;
        return true;
    }
    
    static std::shared_ptr<AnimationData> alloc(int uid) {
        std::shared_ptr<AnimationData> result = std::make_shared<AnimationData>();
        return (result->init(uid) ? result : nullptr);
    }
    
    std::shared_ptr<AnimationState> getAnimationState(std::string state) {
        return _statemap.at(state);
    }
    
    bool eventExists(AnimationEvent event) {
        return _eventmap.find(event) != _eventmap.end();
    }
    
    std::shared_ptr<AnimationUpdate> getAnimationUpdate(AnimationEvent event) {
        return _eventmap.at(event);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    static AnimationEvent stringToEvent(std::string event){
        if (event == "SPAWNING") return AnimationEvent::SPAWNING;
        if (event == "SPAWN") return AnimationEvent::SPAWN;
        if (event == "ACTIVE") return AnimationEvent::ACTIVE;
        if (event == "ATTACK") return AnimationEvent::ATTACK;
        if (event == "RETURN") return AnimationEvent::RETURN;
        if (event == "HIT") return AnimationEvent::HIT;
        if (event == "DEATH") return AnimationEvent::DEATH;
        std::cout << "AnimationData: default event?\n";
        return AnimationEvent::DEFAULT;
    }
};

#endif /* AnimationData_hpp */
