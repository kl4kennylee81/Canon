//
//  AIController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletController_hpp
#define BulletController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Event.hpp"
#include "GameState.hpp"
#include "ActiveBullet.hpp"
#include "WaveData.hpp"
#include "AIData.hpp"

class BulletController : public BaseController {
    
public:
    std::unordered_set<std::shared_ptr<ActiveBullet>> _enemies;
    
    std::unordered_map<GameObject*, std::shared_ptr<ActiveBullet>> _map;
    
    BulletController();
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    virtual bool init();
    
    void addBullet(std::shared_ptr<ActiveBullet> bullet);
    
    void removeBullet(GameObject* obj);
    
    static std::shared_ptr<BulletController> alloc() {
        std::shared_ptr<BulletController> result = std::make_shared<BulletController>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* BulletController_hpp */
