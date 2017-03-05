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

class PathController : public BaseController {
public:

	float _height;
	float _minx;
	float _maxx;
	float _miny;
	float _maxy;
    
    bool _is_moving;

	std::shared_ptr<Path> _path;

	std::shared_ptr<cugl::Node> _pathSceneNode;

	bool _wasPressed;

	bool _touch;

	void addPathToScene(std::shared_ptr<GameState> state);

	void updateMinMax(cugl::Vec2 vec);

	void resetMinMax();
	
    PathController();
    
	virtual void attach(std::shared_ptr<Observer> obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);
    
    /**
     * Update the observer state based on an event from the subject
     */
    virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);

	virtual bool init(std::shared_ptr<GameState> state, bool touch);

	static std::shared_ptr<PathController> alloc(std::shared_ptr<GameState> state, bool touch) {
		std::shared_ptr<PathController> result = std::make_shared<PathController>();
		return (result->init(state, touch) ? result : nullptr);
    }

	cugl::Vec2 getInputVector();

	bool getIsPressed();
};

#endif /* PathController_hpp */
