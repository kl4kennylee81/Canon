//
//  GameplayController.hpp
//  Canon
//
//  Created by Jonathan Chen on 2/24/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameplayController_hpp
#define GameplayController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "World.hpp"
#include "GameState.hpp"
#include "BaseController.hpp"
#include "PathController.hpp"
#include "MoveController.hpp"
#include "CollisionController.hpp"
#include "AIController.hpp"
#include "SwitchController.hpp"
#include "LevelController.hpp"
#include "AnimationController.hpp"
#include "SpawnController.hpp"
#include "ZoneController.hpp"
#include "ClockController.hpp"
#include "ParticleController.hpp"
#include "FinishController.hpp"
#include "SoundController.hpp"
#include "Observer.hpp"
#include "Event.hpp"
#include "BulletController.hpp"
#include "TutorialController.hpp"
#include "ChapterSelectData.hpp"

class GameplayController : public BaseController {
protected:
	std::shared_ptr<GameState> _gameState;
	std::shared_ptr<PathController> _pathController;
	std::shared_ptr<MoveController> _moveController;
	std::shared_ptr<CollisionController> _collisionController;
	std::shared_ptr<AIController> _aiController;
    std::shared_ptr<BulletController> _bulletController;
    std::shared_ptr<SwitchController> _switchController;
    std::shared_ptr<LevelController>  _levelController;
    std::shared_ptr<SpawnController> _spawnController;
    std::shared_ptr<ZoneController> _zoneController;
    std::shared_ptr<AnimationController> _animationController;
    std::shared_ptr<ClockController> _clockController;
    std::shared_ptr<ParticleController> _particleController;
    std::shared_ptr<FinishController> _finishController;
    std::shared_ptr<SoundController> _soundController;
    std::shared_ptr<TutorialController> _tutorialController;

    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _scale;
	bool _paused;

    std::string _curLevelName;
    std::shared_ptr<ChapterSelectData> _chapterData;
    std::shared_ptr<GenericAssetManager> _assets;
    
    bool _nextLevel;
    
public:
    
    GameplayController();
    
    ~GameplayController();

	virtual void attach(Observer* obs);
	virtual void detach(Observer* obs);
	virtual void notify(Event* e);
	virtual void eventUpdate(Event* e);
    virtual void update(float timestep);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    void activate();
    
    void deactivate();
    
    void dispose();
    
    std::shared_ptr<LevelData> getCurrentLevel();
    
    std::shared_ptr<World> getWorld();

    virtual bool init(std::shared_ptr<cugl::Scene> scene, std::shared_ptr<World> levelWorld, std::shared_ptr<ChapterSelectData> chapterData, std::string curLevelName);

    static std::shared_ptr<GameplayController> alloc(std::shared_ptr<cugl::Scene> scene, std::shared_ptr<World> levelWorld, std::shared_ptr<ChapterSelectData> chapterData, std::string curLevelName) {
		std::shared_ptr<GameplayController> result = std::make_shared<GameplayController>();
		return (result->init(scene,levelWorld, chapterData,curLevelName) ? result : nullptr);
	}

    std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    void onResume(const std::shared_ptr<cugl::JsonValue>);
    
    bool chapterHasAnotherLevel();
    
    std::string getNextLevelName();

};

#endif /* GameplayController_hpp */
