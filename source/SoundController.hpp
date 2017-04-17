//
//  SoundController.hpp
//  Canon
//
//  Created by Jonathan Chen on 4/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SoundController_hpp
#define SoundController_hpp

#include <stdio.h>

#include <cugl/cugl.h>
#include <map>
#include "BaseController.hpp"
#include "World.hpp"

class SoundController : public BaseController {
protected:
    int step;
    std::shared_ptr<World> _world;
    
    std::shared_ptr<SoundData> generalSounds;
    std::map<GameObject*, std::shared_ptr<SoundData>> objectSoundMap;
    
public:
    SoundController();
    
    ~SoundController(){ dispose(); }
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    void dispose() {};
    
    virtual bool init(std::shared_ptr<World> world);
    
    static std::shared_ptr<SoundController> alloc(std::shared_ptr<World> world) {
        std::shared_ptr<SoundController> result = std::make_shared<SoundController>();
        return (result->init(world) ? result : nullptr);
    }
    
    void handleObjectAction(GameObject* obj, SoundAction action);
    
    void handleGeneralAction(SoundAction action);
};

#endif /* SoundController_hpp */
