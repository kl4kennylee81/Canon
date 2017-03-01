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
protected:
    std::shared_ptr<cugl::Obstacle> _body;
    float _speed;
    float _acceleration;
    Element _elementType;
public:
    
    cugl::Obstacle* getBody();
    
};

#endif /* PhysicsComponent_hpp */
