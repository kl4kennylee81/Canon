//
//  PathController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathController_hpp
#define PathController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "Observer.hpp"
#include "Event.hpp"
#include "GameState.hpp"
#include "Path.hpp"
#include "World.hpp"

enum PathControllerState
{
    IDLE = 0,
    DRAWING,
    MOVING
};


class PathController : public BaseController {
public:
    PathController();
    
    ~PathController() { dispose(); };

	float _height;
	float _minx;
	float _maxx;
	float _miny;
	float _maxy;
    
    /**
     * The number of frames progressed so far for cooldown
     */
    float _cooldown_frames;
    
    //Do not start looking for input until player object has spawned
    bool _spawnStart;
    
    std::shared_ptr<World> _world;
    
    PathControllerState controllerState;

	std::shared_ptr<Path> _path;

	std::shared_ptr<cugl::Node> _pathSceneNode;

	bool _wasPressed;
    
    std::unordered_set<std::shared_ptr<GameObject>> _movingChars;
    
    std::shared_ptr<GameObject> _drawingChar;
    
    std::shared_ptr<cugl::Texture> _mainTexture;
    
    std::shared_ptr<cugl::Texture> _capTexture;

	void addPathToScene(std::shared_ptr<GameState> state);

    void updateMinMax(cugl::Vec2 vec);
    
    
    bool isOnCooldown();

	void resetMinMax();
    
    std::shared_ptr<GameObject> getActiveCharacter(std::shared_ptr<GameState> state);

    virtual void attach(Observer* obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);
    
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep, std::shared_ptr<GameState> state);
    
    void dispose();
    
    void setTextures(std::shared_ptr<cugl::Texture> main, std::shared_ptr<cugl::Texture> cap){
        _mainTexture = main;
        _capTexture = cap;
    }

    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<World> world);

    static std::shared_ptr<PathController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
		std::shared_ptr<PathController> result = std::make_shared<PathController>();
		return (result->init(state, world) ? result : nullptr);
    }
};

#endif /* PathController_hpp */
