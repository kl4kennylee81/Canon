//
//  CollisionController.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef CollisionController_hpp
#define CollisionController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_map>
#include <vector>
#include <map>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2Body.h>
#include "BaseController.hpp"
#include "GameObject.hpp"
#include "Observer.hpp"
#include "Event.hpp"
#include "GameState.hpp"
#include "LevelEvent.hpp"
#include "ZoneEvent.hpp"

class CollisionController : public BaseController {
protected:
    bool inGame;
    /** The Box2D world */
    std::shared_ptr<cugl::ObstacleWorld> _world;
    
    std::shared_ptr<cugl::Node> _debugnode;
    
    std::shared_ptr<cugl::Texture> _arrowTexture;
    
    std::vector<GameObject*> objsScheduledForRemoval;
    std::vector<GameObject*> objsToIgnore;
    
    std::map<GameObject*,float> hitStunMap;
    
    GameObject* bluePlayer;
    GameObject* goldPlayer;
    GameObject* blueZone;
    GameObject* goldZone;
    
    /** Whether or not debug mode is active */
    bool _debug;
    
    /**
     * Returns true if debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @return true if debug mode is active.
     */
    bool isDebug( ) const { return _debug; }
    
    /**
     * Sets whether debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @param value whether debug mode is active.
     */
    void setDebug(bool value) {
        _debug = value;
        _debugnode->setVisible(value);
    }

public:
    
    CollisionController();
    
    ~CollisionController() { dispose(); }

	virtual void attach(Observer* obs);
	virtual void detach(Observer* obs);
	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);
    
    virtual void update(float timestep,std::shared_ptr<GameState> state);
    
    void updateHitStun();
    
    void dispose();

	virtual bool init();

    virtual bool init(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets);
    
    void initPhysicsComponent(ObjectInitEvent* objectInit);
    
    void initPhysicsComponent(ZoneInitEvent* zoneInit);

    bool addToWorld(GameObject* obj);
    
    bool removeFromWorld(GameObject* obj);
    
	static std::shared_ptr<CollisionController> alloc() {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init() ? result : nullptr);
	}

    static std::shared_ptr<CollisionController> alloc(std::shared_ptr<GameState> state, std::shared_ptr<GenericAssetManager> assets) {
		std::shared_ptr<CollisionController> result = std::make_shared<CollisionController>();
		return (result->init(state, assets) ? result : nullptr);
	}
    
    void beginContact(b2Contact* contact);
    
    void beforeSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif /* CollisionController_hpp */
