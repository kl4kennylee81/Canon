//
//  BulletAI.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletAI_hpp
#define BulletAI_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"

class BulletAI : public ActiveAI {
private:
    bool _isActive;
    float velocity;
    float angle;
    std::shared_ptr<GameObject> _object;
public:
    
    BulletAI(){}
    
    ~BulletAI(){ dispose(); }
    
    void dispose(){
        _object = nullptr;
    }
    
    void update(std::shared_ptr<GameState> state);
    
    bool garbageCollect(GameObject* obj);
    
    std::vector<std::shared_ptr<GameObject>> getObjects() {
        std::vector<std::shared_ptr<GameObject>> objects;
        objects.push_back(_object);
        return objects;
    }
    
    bool isActive();
    
    void toggleActive();
    
    bool init(std::shared_ptr<GameObject> object, float velocity, float angle) {
        
        _object = object;
        _isActive = false;
        this->velocity = velocity;
        this->angle = angle;
        return true;
    }
    
    static std::shared_ptr<BulletAI> alloc(std::shared_ptr<GameObject> object, float velocity, float angle) {
        std::shared_ptr<BulletAI> result = std::make_shared<BulletAI>();
        return (result->init(object, velocity, angle) ? result : nullptr);
    }
    
    static std::shared_ptr<BulletAI> allocWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state)
    {
        std::shared_ptr<BulletAI> result = std::make_shared<BulletAI>();
        return (result->initWithJson(json, state) ? result : nullptr);
    }
    
    virtual std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    bool initWithJson(std::shared_ptr<cugl::JsonValue> json, std::shared_ptr<GameState> state);
};

#endif /* BulletAI_hpp */
