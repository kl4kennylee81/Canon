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
#include <ObjectData.hpp>
#include <AnimationData.hpp>
#include <PathData.hpp>
#include <ShapeData.hpp>
#include <unordered_map>

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

	std::unordered_map<int, std::shared_ptr<ObjectData>> _objectData;
	std::unordered_map<int, std::shared_ptr<AnimationData>> _animationData;
	std::unordered_map<int, std::shared_ptr<PathData>> _pathData;
	std::unordered_map<int, std::shared_ptr<ShapeData>> _shapeData;


public:

	virtual bool init() {
		return true;
	}

	static std::shared_ptr<World> alloc() {
		std::shared_ptr<World> result = std::make_shared<World>();
		return (result->init() ? result : nullptr);
	}
};

#endif /* World_hpp */
