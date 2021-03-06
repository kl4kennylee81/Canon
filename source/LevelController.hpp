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
#include "LevelEnums.hpp"



class LevelController : public BaseController {
protected:
    std::shared_ptr<World> _world;
    
    Level _level;
    
    std::shared_ptr<ProgressBarController> _progressBarController;

	std::map<int, std::string> _uidToWaveEntryMap;
    
    LevelState _levelState;
    
    bool _spawnPlayers;

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

    std::shared_ptr<GameObject> spawnWaveEntry(std::shared_ptr<WaveEntry> we, bool isPlayer,std::shared_ptr<GameState> state);
    
    std::shared_ptr<GameObject> spawnAndRecordWaveEntry(std::shared_ptr<WaveEntry> we,
                                                        bool isPlayer,std::shared_ptr<GameState> state);
    
    std::shared_ptr<GameObject> spawnAndNotifyWaveEntry(std::shared_ptr<WaveEntry> we,
                                                        bool isPlayer,std::shared_ptr<GameState> state, float spawnTime);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    void dispose();
    
    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<World> world, bool spawnPlayers);
    
    bool init(std::string levelDataKey, std::shared_ptr<GameState> state, std::shared_ptr<World> world);
    
    static std::shared_ptr<LevelController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<World> world, bool spawnPlayers = true) {
        std::shared_ptr<LevelController> result = std::make_shared<LevelController>();
        return (result->init(state, world, spawnPlayers) ? result : nullptr);
    }

	std::shared_ptr<World> getWorld() {
		return _world;
	}
    
	void initAfterResume(std::shared_ptr<GameState> state,
                         std::shared_ptr<cugl::JsonValue> levelControlJson,
                         std::shared_ptr<cugl::JsonValue> spawnControlJson);

	std::string serialize(std::shared_ptr<GameState> state);

    std::shared_ptr<cugl::JsonValue> toJsonValue(std::shared_ptr<GameState> state);
    
    void removeProgressBarNode() { _progressBarController->cleanup();};
};

#endif /* LevelController_hpp */
