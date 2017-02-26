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
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2Body.h>
#include "BaseController.hpp"
#include "GameObject.hpp"

class CollisionController : BaseController {
protected:
    /** The Box2D world */
    std::shared_ptr<cugl::ObstacleWorld> _world;
};

#endif /* CollisionController_hpp */
