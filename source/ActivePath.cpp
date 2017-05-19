//
//  ActivePath.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActivePath.hpp"
#include "PathParameters.h"

using namespace cugl;

bool ActivePath::updateActivePath(cugl::Vec2 newPos,float radius){
    if (_pathIndex >= _path->size()) {
        return true;
    }
    Vec2 goal = _path->get(_pathIndex);
    Vec2 current = newPos;
    //
    if (std::abs(current.distance(goal)) <= radius) {
        _pathIndex = _pathIndex + 1;
        
        // updated active path is now completed
        if (_pathIndex >= _path->size()) {
            return true;
        }
    }
    return false;
}
