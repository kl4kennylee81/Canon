//
//  ActivePath.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActivePath_hpp
#define ActivePath_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Path.hpp"
#include "PathParameters.h"

class ActivePath {
public:
    std::shared_ptr<Path> _path;
    int _pathIndex;

	bool init(std::shared_ptr<Path> path) {
		_path = path;
		_pathIndex = 0;
		return true;
	}

	ActivePath():_path(nullptr),_pathIndex(0) {

	}

	bool init(std::shared_ptr<Path> path, int index) {
		_path = path;
		_pathIndex = index;
		return true;
	}

	static std::shared_ptr<ActivePath> alloc(std::shared_ptr<Path> path) {
		std::shared_ptr<ActivePath> result = std::make_shared<ActivePath>();
		return (result->init(path) ? result : nullptr);
	}

	static std::shared_ptr<ActivePath> alloc(std::shared_ptr<Path> path, int index) {
		std::shared_ptr<ActivePath> result = std::make_shared<ActivePath>();
		return (result->init(path, index) ? result : nullptr);
	}
    
    /** returns true if the active path is completed */
    bool updateActivePath(cugl::Vec2 newPos, float radius=RADIUS_WORLD);
};

#endif /* ActivePath_hpp */
