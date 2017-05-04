//
//  LevelController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelController_hpp
#define LevelController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "GameState.hpp"
#include "World.hpp"
#include "Level.hpp"
#include <map>
#include "ProgressBarController.hpp"

class LevelController : BaseController {
protected:
    std::shared_ptr<World> _world;
    
    Level _level;
    
    std::shared_ptr<ProgressBarController> _progressBarController;

	std::map<int, std::string> _uidToWaveEntryMap;

public:
    LevelController();
    
    ~LevelController() { dispose(); };
    
    virtual void attach(Observer* obs);
    
    virtual void detach(Observer* obs);
    
    virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);

    void spawnWaveEntry(std::shared_ptr<WaveEntry> we, bool isPlayer,std::shared_ptr<GameState> state);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    void dispose();
    
	void resumeHelper(std::shared_ptr<GameState> state, std::shared_ptr<cugl::JsonValue> resumeJson, std::string key);
    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<World> world);
    
    static std::shared_ptr<LevelController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
        std::shared_ptr<LevelController> result = std::make_shared<LevelController>();
        return (result->init(state, world) ? result : nullptr);
    }

	std::shared_ptr<World> getWorld() {
		return _world;
	}
    
	void initAfterResume(std::shared_ptr<GameState> state, std::shared_ptr<cugl::JsonValue> resumeJson);
	std::string serialize(std::shared_ptr<GameState> state);
    std::shared_ptr<cugl::JsonValue> toJsonValue(std::shared_ptr<GameState> state);
};

#endif /* LevelController_hpp */
