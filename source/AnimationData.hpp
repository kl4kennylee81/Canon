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
#include "AnimationAction.hpp"

class AnimationState {
public:
    int first;
    int total;
    std::vector<int> frames;
    float alpha;
    
    bool init(int f, int t, std::vector<int> fs, float a) {
        first = f;
        total = t;
        frames = fs;
        alpha = a;
        return true;
    }
    
    static std::shared_ptr<AnimationState> alloc(int f, int t, std::vector<int> fs, float a) {
        std::shared_ptr<AnimationState> result = std::make_shared<AnimationState>();
        return (result->init(f,t,fs,a) ? result : nullptr);
    }

	std::shared_ptr<cugl::JsonValue> toJsonValue();
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

	std::shared_ptr<cugl::JsonValue> toJsonValue();
};

class AnimationData : public Data {
protected:
    std::map<std::string, std::shared_ptr<AnimationState>> _statemap;
    
    std::map<AnimationAction, std::shared_ptr<AnimationUpdate>> _actionmap;
    
public:
    std::shared_ptr<cugl::Texture> texture;
    int rows;
    int cols;
    int size;
    bool nonUniformScale;
    float angle;
    bool physicsAngle;
    
    AnimationData() : Data(){}
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<AnimationData> alloc() {
        std::shared_ptr<AnimationData> result = std::make_shared<AnimationData>();
        return (result->init() ? result : nullptr);
    }
    
    std::shared_ptr<AnimationState> getAnimationState(std::string state) {
        return _statemap.at(state);
    }
    
    bool actionExists(AnimationAction action) {
        return _actionmap.find(action) != _actionmap.end();
    }
    
    std::shared_ptr<AnimationUpdate> getAnimationUpdate(AnimationAction action) {
        return _actionmap.at(action);
    }

	std::map<std::string, std::shared_ptr<AnimationState>> getStateMap()
	{
		return _statemap;
	}
    
	std::map<AnimationAction, std::shared_ptr<AnimationUpdate>> getActionMap()
	{
		return _actionmap;
	}

    virtual std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    virtual bool preload(const std::string& file) override;
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    virtual bool materialize() override;

};

#endif /* AnimationData_hpp */
