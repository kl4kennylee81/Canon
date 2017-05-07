//
//  ActiveBullet.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//
#ifndef ActiveBullet_hpp
#define ActiveBullet_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameState.hpp"
#include "ActiveAI.hpp"
#include "GameObject.hpp"
#include "BulletData.hpp"
#include "BulletInitEvent.hpp"

class ActiveBullet {
private:
    bool _isActive;
    int _frameCount;
    std::shared_ptr<GameObject> _object;
    std::shared_ptr<BulletData> _bulletData;
    
public:
    
    ActiveBullet(){}
    
    ~ActiveBullet(){ dispose(); }
    
    void dispose(){
        _object = nullptr;
    }
    
    std::vector<std::shared_ptr<BulletInitEvent>> update(std::shared_ptr<GameState> state);
    
    std::vector<std::shared_ptr<BulletInitEvent>> shootBullets(std::shared_ptr<GameState> state);
    
    bool garbageCollect(GameObject* obj) { return true; }
    
    std::vector<std::shared_ptr<GameObject>> getObjects() {
        std::vector<std::shared_ptr<GameObject>> objects;
        objects.push_back(_object);
        return objects;
    }
    
    bool isActive() {return _isActive;}
    
    void toggleActive() { _isActive = !_isActive;  }
    
    bool init(std::shared_ptr<GameObject> object, std::shared_ptr<BulletData> data) {
        _object = object;
        _isActive = false;
        _frameCount = 0;
        _bulletData = data;
        return true;
    }
    
    static std::shared_ptr<ActiveBullet> alloc(std::shared_ptr<GameObject> object, std::shared_ptr<BulletData> data) {
        std::shared_ptr<ActiveBullet> result = std::make_shared<ActiveBullet>();
        return (result->init(object, data) ? result : nullptr);
    }
};

#endif /* ActiveBullet_hpp */
