//
//  BulletAIData.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletAIData_hpp
#define BulletAIData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AIData.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class BulletAIData : public AIData {
public:
    float velocity;
    
    float angle;
    
    BulletAIData() : AIData() {}
    
    bool init(float velocity, float angle) {
        this->velocity = velocity;
        this->angle = angle;
        return true;
    }
    
    static std::shared_ptr<BulletAIData> alloc(float velocity, float angle) {
        std::shared_ptr<BulletAIData> result = std::make_shared<BulletAIData>();
        return (result->init(velocity, angle) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::JsonValue> toJsonValue() override;
    
    bool preload(const std::string& file) override;
    
    bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    bool materialize() override;
    
    std::shared_ptr<ActiveAI> newActiveAI(std::shared_ptr<GameObject> object) override;
};

#endif /* BulletAIData_hpp */
