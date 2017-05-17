//
//  GameState.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameObject.hpp"
#include "GenericAssetManager.hpp"
#include "InternalClock.hpp"
#include "GameStateEnums.hpp"

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_SCENE_WIDTH 1024
/** This is the aspect ratio for physics */
//#define GAME_SCENE_ASPECT (9.0/16.0)

/** This is the logical physics width of the game **/
#define GAME_PHYSICS_WIDTH 32.f

/** This is the logical physics height of the game **/
#define GAME_PHYSICS_HEIGHT 16.f

/** The scaling from physics coordinates to the game scene **/
#define GAME_PHYSICS_SCALE (GAME_SCENE_WIDTH/GAME_PHYSICS_WIDTH)

class GameState {
protected:
    /** Reference to the root of our scene graph.
     * Example:
     * Child1 : _gameplayNode
     * Child2 : _menuNode
     */
    std::shared_ptr<cugl::Scene>  _scene;
    
    /** the root node to all nodes in the gameState **/
    std::shared_ptr<cugl::Node>   _gameplayNode;
    
    /** Reference to the physics world root of the scene graph
     *  All physics objects are in this
     */
    std::shared_ptr<cugl::Node> _worldnode;
    
    std::shared_ptr<cugl::Node> _debugnode;
    
    std::shared_ptr<cugl::Node> _bgnode;
    
    std::shared_ptr<cugl::Node> _bordernode;
    
    cugl::Rect _bounds;
    
    std::vector<std::shared_ptr<GameObject>> _playerCharacters;
    
    int _activeCharacterPosition;

    
    /**
     * All objects include the player characters and all other objects in the game
     */
    std::vector<std::shared_ptr<GameObject>> _enemyObjects;
    
    bool _reset;
    
    GameplayState _state; // the state machine of the gameplay controller
public:
    static std::unique_ptr<InternalClock> _internalClock;
    
GameState():
    _reset(false),
    _worldnode(nullptr),
    _debugnode(nullptr),
    _bgnode(nullptr),
    _scene(nullptr),
    _bounds(cugl::Rect()),
    _activeCharacterPosition(0){}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~GameState() { dispose(); }
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene, const std::shared_ptr<GenericAssetManager>& assets);
    
    void dispose();

    void attachToScene();
    
    void detachFromScene();

    static std::shared_ptr<GameState> alloc(std::shared_ptr<cugl::Scene> scene, const std::shared_ptr<GenericAssetManager>& assets) {
		std::shared_ptr<GameState> result = std::make_shared<GameState>();
		return (result->init(scene, assets) ? result : nullptr);
	}
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    /**
     * Return the vector by reference
     */
    std::vector<std::shared_ptr<GameObject>>& getPlayerCharacters() { return _playerCharacters; }

	std::shared_ptr<cugl::Scene>& getScene() { return _scene; }
    
    std::shared_ptr<cugl::Node>& getGameplayNode() { return _gameplayNode; };

    std::shared_ptr<GameObject> getActiveCharacter();

    std::shared_ptr<GameObject> getInactiveCharacter();
    
    std::shared_ptr<GameObject> getOtherPlayer(std::shared_ptr<GameObject> player);
    
    std::vector<std::shared_ptr<GameObject>> getInactiveCharacters();

    bool getReset();
    
    void toggleReset();
    
    void toggleActiveCharacter();
    
    std::vector<std::shared_ptr<GameObject>>& getEnemyObjects() { return _enemyObjects; }
    
    std::shared_ptr<cugl::Node> getDebugNode() {
        return _debugnode;
    }
    
    cugl::Rect getRect() {
        return _bounds;
    }
    
    void addEnemyGameObject(std::shared_ptr<GameObject> obj);
    
    void addPlayerGameObject(std::shared_ptr<GameObject> obj);
    
    std::shared_ptr<GameObject> getPlayer(int index);
    
    size_t getNumberPlayerCharacters();
    
    size_t getNumberEnemyCharacters();
    
    size_t getNumberNonBulletEnemyCharacters();
    
    std::shared_ptr<GameObject> getClosestChar(cugl::Vec2 world_coord);
    
    void setClosestChar(cugl::Vec2 world_coord);
    
    std::shared_ptr<cugl::Node> getWorldNode() {
        return _worldnode;
    }
    
    void removeObject(GameObject* obj) {
        for(auto it = _playerCharacters.begin() ; it != _playerCharacters.end(); ++it) {
            if (it->get() == obj){
                it = _playerCharacters.erase(it);
                break;
            }
        }
        for(auto it = _enemyObjects.begin() ; it != _enemyObjects.end(); ++it) {
            if (it->get() == obj){
                it = _enemyObjects.erase(it);
                break;
            }
        }
    }

	std::shared_ptr<GameObject> getUID2GameObject(int uid);
    
#pragma worldNode Transformation Settings getter
    float getPhysicsScale();
};

#endif /* GameState_hpp */
