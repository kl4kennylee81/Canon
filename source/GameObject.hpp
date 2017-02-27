//
//  GameObject.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "PhysicsComponent.hpp"

class GameObject {
    
protected:
    int uid;
    std::unique_ptr<PhysicsComponent> _body;
    std::shared_ptr<cugl::Node> _node;
};

#endif /* GameObject_hpp */
