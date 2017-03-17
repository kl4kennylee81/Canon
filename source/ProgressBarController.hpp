//
//  ProgressBarController.hpp
//  Canon
//
//  Created by Kelly Yu on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ProgressBarController_hpp
#define ProgressBarController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Observer.hpp"
#include "Event.hpp"
#include "GameState.hpp"
#include "World.hpp"
#include "Level.hpp"
#include "LevelData.hpp"
#include "GenericAssetManager.hpp"

class ProgressBarController : public BaseController
{
private:
	// child of the Scene, parent of the pBars
	std::shared_ptr<cugl::Node> _pBarSceneNode;

public:
	ProgressBarController();
	~ProgressBarController();

	// inherited from base
	virtual void attach(std::shared_ptr<Observer> obs);
	virtual void detach(Observer* obs);
	virtual void notify(Event* e);
	virtual void eventUpdate(Event* e);

    virtual void update(std::shared_ptr<GameState> state,Level level);

    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<World> world);
    
    void cleanup();
    
    bool reset(std::shared_ptr<GameState> state, std::shared_ptr<World> world);

    static std::shared_ptr<ProgressBarController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
		std::shared_ptr<ProgressBarController> result = std::make_shared<ProgressBarController>();
		return (result->init(state, world) ? result : nullptr);
	}
};

#endif /* ProgressBarController_hpp */

