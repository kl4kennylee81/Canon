//
//  World.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_map>
#include "ObjectData.hpp"
#include "AnimationData.hpp"
#include "PathData.hpp"
#include "ShapeData.hpp"
#include "WaveData.hpp"
#include "LevelData.hpp"

/** contain all the static data loaded in metadata needed/ prototypes of
  * path data, physics shape data, animation data etc. to spawn out the active
  * version in game
  *
  * ex. shape data needed to create a box2d Obstacle
  * ex. path data for the prespecified movement to generate the active path 
  *     in the the enemy AI controller

  */

class World {

protected:
    // the level of the world
    std::shared_ptr<LevelData> _levelData;
    
    /** The asset manager for this game world. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    // static prototypes used to spawn units into the gameState
	std::unordered_map<int, std::shared_ptr<ObjectData>> _objectData;
	std::unordered_map<int, std::shared_ptr<AnimationData>> _animationData;
	std::unordered_map<int, std::shared_ptr<PathData>> _pathData;
	std::unordered_map<int, std::shared_ptr<ShapeData>> _shapeData;
	std::unordered_map<int, std::shared_ptr<WaveData>> _waveData;

public:
    
    World();
    
    ~World();
    
    virtual bool init(){
        return true;
    }

    virtual bool init(std::shared_ptr<cugl::AssetManager> assets);
    
    virtual bool init(std::shared_ptr<cugl::AssetManager> assets, std::shared_ptr<LevelData> levelData){
        _assets = assets;
        _levelData = levelData;
        return true;
    }
    
    std::shared_ptr<cugl::AssetManager> getAssetManager();

	static std::shared_ptr<World> alloc() {
		std::shared_ptr<World> result = std::make_shared<World>();
		return (result->init() ? result : nullptr);
	}

    static std::shared_ptr<World> alloc(std::shared_ptr<cugl::AssetManager> assets) {
        std::shared_ptr<World> result = std::make_shared<World>();
        return (result->init(assets) ? result : nullptr);
    }
    
    static std::shared_ptr<World> alloc(std::shared_ptr<cugl::AssetManager> assets,std::shared_ptr<LevelData> levelData) {
        std::shared_ptr<World> result = std::make_shared<World>();
        return (result->init(assets,levelData) ? result : nullptr);
    }
    
    std::shared_ptr<LevelData> getLevelData(){
        return _levelData;
    }
    
    /** testing function to populate the world without the data files */
    void populate();
};

#endif /* World_hpp */
