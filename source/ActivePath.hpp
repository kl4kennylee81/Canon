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

	static std::shared_ptr<ActivePath> alloc(std::shared_ptr<Path> path) {
		std::shared_ptr<ActivePath> result = std::make_shared<ActivePath>();
		return (result->init(path) ? result : nullptr);
	}
};

#endif /* ActivePath_hpp */
