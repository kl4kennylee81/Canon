//
//  ZoneController.hpp
//  Canon
//
//  Created by Jonathan Chen on 3/22/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ZoneController_hpp
#define ZoneController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "BaseController.hpp"
#include "GameObject.hpp"
#include "ActiveZone.hpp"
#include "World.hpp"
#include "GameState.hpp"

class ZoneController : public BaseController {
protected:
    std::shared_ptr<World> _world;
    std::shared_ptr<GameState> state;
    
    std::map<GameObject*, std::shared_ptr<ActiveZone>> zoneMap;
    
    std::vector<GameObject*> objsScheduledForRemoval;
    
public:
    ZoneController();
    
    ~ZoneController(){ dispose(); }
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep);
    
    void dispose();
    
    virtual bool init(std::shared_ptr<GameState> state,std::shared_ptr<World> world) {
        this->state = state;
        this->_world = world;
        return true;
    }
    
    static std::shared_ptr<ZoneController> alloc(std::shared_ptr<GameState> state,std::shared_ptr<World> world) {
        std::shared_ptr<ZoneController> result = std::make_shared<ZoneController>();
        return (result->init(state,world) ? result : nullptr);
    }
    
    void updateStaticZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<StaticZoneData> data, std::vector<GameObject*> zoneObjs);
    
    void updateRotateZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<RotateZoneData> data, std::vector<GameObject*> zoneObjs);
    
    void updatePulseZone(GameObject* charObj, std::shared_ptr<ActiveZone> zone, std::shared_ptr<PulseZoneData> data, std::vector<GameObject*> zoneObjs);
    
    void addToMap(GameObject* obj, std::vector<std::shared_ptr<ZoneData>> datas);
    
    void staticZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<StaticZoneData> data, GameObject* gameObj);
    
    void rotateZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<RotateZoneData> data, GameObject* gameObj);
    
    void pulseZoneInit(std::shared_ptr<ActiveZone> activeZone, std::shared_ptr<PulseZoneData> data, GameObject* gameObj);
    
    void handleObjectSpawning(GameObject* obj);
    
    void handleObjectSpawn(GameObject* obj);
    
    void removeObjectZones(GameObject* obj);
    
    void toggleZone(std::shared_ptr<ActiveZone> zone);
};

#endif /* ZoneController_hpp */
