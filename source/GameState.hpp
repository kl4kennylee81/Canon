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

class GameState {
protected:
    /** Reference to the physics world root of the scene graph 
     *  All physics objects are in this
     */
    std::shared_ptr<cugl::Node> _worldnode;
    
    std::shared_ptr<cugl::Node> _debugnode;
    
    std::shared_ptr<cugl::Node> _bgnode;
    
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
    std::vector<std::shared_ptr<GameObject>> _allObjects;
    
    /** not sure yet if representing time as an int is the move */
    int time_elapsed;

public:
    virtual bool init(const std::shared_ptr<cugl::AssetManager>& assets);

	static std::shared_ptr<GameState> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
		std::shared_ptr<GameState> result = std::make_shared<GameState>();
		return (result->init(assets) ? result : nullptr);
	}
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
        
    /**
     * Return the vector by reference
     */
    std::vector<std::shared_ptr<GameObject>>& getPlayerCharacters() { return _playerCharacters; }

	std::shared_ptr<cugl::Scene>& getScene() { return _scene; }

	std::shared_ptr<GameObject> getActiveCharacter() { return _playerCharacters.at(_activeCharacterPosition); }
    
    std::vector<std::shared_ptr<GameObject>>& getAllObjects() { return _allObjects; }
    
    std::shared_ptr<cugl::Node> getDebugNode() {
        return _debugnode;
    }
};

#endif /* GameState_hpp */
