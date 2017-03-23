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

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_SCENE_WIDTH 1024
/** This is the aspect ratio for physics */
#define GAME_SCENE_ASPECT (9.0/16.0)

/** This is the logical physics width of the game **/
#define GAME_PHYSICS_WIDTH 32.f

/** This is the logical physics height of the game **/
#define GAME_PHYSICS_HEIGHT 18.f

/** The scaling from physics coordinates to the game scene **/
#define GAME_PHYSICS_SCALE (GAME_SCENE_WIDTH/GAME_PHYSICS_WIDTH)

class GameState {
protected:
    /** Reference to the physics world root of the scene graph 
     *  All physics objects are in this
     */
    std::shared_ptr<cugl::Node> _worldnode;
    
    std::shared_ptr<cugl::Node> _debugnode;
    
    std::shared_ptr<cugl::Node> _bgnode;
    
    cugl::Rect _bounds;
    
    /** The root of our scene graph. 
     * Example:
     * Child1 : Background scene node
     * Child2 : is the _worldnode for the physics world root
     * would be used to define a draw order in the z axis for the different layers
     */
    std::shared_ptr<cugl::Scene>  _scene;
    
    std::vector<std::shared_ptr<GameObject>> _playerCharacters;
    
    int _activeCharacterPosition;

    
    /**
     * All objects include the player characters and all other objects in the game
     */
    std::vector<std::shared_ptr<GameObject>> _enemyObjects;
public:
    
    /* TODO temporary will remove once level loading is done */
    bool reset;
    
    /* Need to multiply this scale by physics coordinates to get world coordinates */
    static float _physicsScale;
    
    virtual bool init(const std::shared_ptr<GenericAssetManager>& assets);

	static std::shared_ptr<GameState> alloc(const std::shared_ptr<GenericAssetManager>& assets) {
		std::shared_ptr<GameState> result = std::make_shared<GameState>();
		return (result->init(assets) ? result : nullptr);
	}
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
        
    /**
     * Return the vector by reference
     */
    std::vector<std::shared_ptr<GameObject>>& getPlayerCharacters() { return _playerCharacters; }

	std::shared_ptr<cugl::Scene>& getScene() { return _scene; }

    std::shared_ptr<GameObject> getActiveCharacter();

	void toggleActiveCharacter() { _activeCharacterPosition = _activeCharacterPosition == 0 ? 1 : 0; }
    
    std::vector<std::shared_ptr<GameObject>>& getEnemyObjects() { return _enemyObjects; }
    
    
    
    std::shared_ptr<cugl::Node> getDebugNode() {
        return _debugnode;
    }
    
    cugl::Rect getRect() {
        return _bounds;
    }
    
    void addEnemyGameObject(std::shared_ptr<GameObject> obj);
    
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
    
#pragma worldNode Transformation Settings getter
    /** getter for the physicsScale to transform from physics to World Coordinates */
    static float getPhysicsScale(){ return GameState::_physicsScale; }
    
    /** the y translate from the scene coordinates bottom left (0,0) to the world coordinates bottom left (0,0) */
    float getSceneToWorldTranslateY();
    
#pragma mark Coordinate Conversions
    /** Physics Coordinate Conversions **/
    cugl::Vec2& physicsToWorldCoords(cugl::Vec2& physicsCoords,cugl::Vec2& worldCoords);
    
    cugl::Vec2& physicsToSceneCoords(cugl::Vec2& physicsCoords,cugl::Vec2& sceneCoords);
    
    cugl::Vec2& physicsToScreenCoords(cugl::Vec2& physicsCoords,cugl::Vec2& screenCoords);
    
    
    /** Screen Coordinate Conversions **/
    cugl::Vec2& screenToWorldCoords(cugl::Vec2& screenCoords, cugl::Vec2& worldCoords);
    
    cugl::Vec2& screenToSceneCoords(cugl::Vec2& screenCoords, cugl::Vec2& sceneCoords);
    
    cugl::Vec2& screenToPhysicsCoords(cugl::Vec2& screenCoords, cugl::Vec2& physicsCoords);
    
    /** World Coordinate Conversions **/
    
    cugl::Vec2& worldToSceneCoords(cugl::Vec2& worldCoords, cugl::Vec2& sceneCoords);
    
    cugl::Vec2& worldToScreenCoords(cugl::Vec2& worldCoords, cugl::Vec2& screenCoords);
    
    cugl::Vec2& worldToPhysicsCoords(cugl::Vec2& worldCoords, cugl::Vec2& physicsCoords);
    
    
    /** Scene Coordinate Conversion **/
    cugl::Vec2& sceneToWorldCoords(cugl::Vec2& sceneCoords, cugl::Vec2& worldCoords);
    
    cugl::Vec2& sceneToScreenCoords(cugl::Vec2& sceneCoords, cugl::Vec2& screenCoords);
    
    cugl::Vec2& sceneToPhysicsCoords(cugl::Vec2& sceneCoords, cugl::Vec2& physicsCoords);
};

#endif /* GameState_hpp */
