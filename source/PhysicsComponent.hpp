//
//  PhysicsComponent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PhysicsComponent_hpp
#define PhysicsComponent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Element.hpp"

class PhysicsComponent {
    std::shared_ptr<cugl::Obstacle> _body;
    float speed;
    float acceleration;
    Element elementType;
};

#endif /* PhysicsComponent_hpp */
