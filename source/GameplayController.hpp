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
#include "Observer.hpp"
#include "Event.hpp"

class GameplayController : public BaseController {
protected:
    
	std::shared_ptr<World> _world;
	std::shared_ptr<GameState> _gameState;
	std::shared_ptr<PathController> _pathController;
	std::shared_ptr<MoveController> _moveController;
	std::shared_ptr<CollisionController> _collisionController;
	std::shared_ptr<AIController> _aiController;
    std::shared_ptr<SwitchController> _switchController;

    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _scale;

public:
    
    GameplayController();

	virtual void attach(std::shared_ptr<Observer> obs);
	virtual void detach(Observer* obs);
	virtual void notify(Event* e);
	virtual void eventUpdate(Event* e);
    virtual void update(float timestep);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);

    virtual bool init(std::shared_ptr<World> levelWorld);

	static std::shared_ptr<GameplayController> alloc(std::shared_ptr<World> levelWorld) {
		std::shared_ptr<GameplayController> result = std::make_shared<GameplayController>();
		return (result->init(levelWorld) ? result : nullptr);
	}


};

#endif /* GameplayController_hpp */
