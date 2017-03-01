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
#include <memory>
#include "Element.hpp"

class PhysicsComponent {
protected:
    std::shared_ptr<cugl::Obstacle> _body;
    float _speed;
    float _acceleration;
    Element _elementType;
public:
    
    PhysicsComponent();
    
    cugl::Obstacle* getBody();
    
    virtual bool init(std::shared_ptr<cugl::Obstacle> body);
    
    static std::shared_ptr<PhysicsComponent> alloc(std::shared_ptr<cugl::Obstacle> body) {
        std::shared_ptr<PhysicsComponent> result = std::make_shared<PhysicsComponent>();
        return (result->init(body) ? result : nullptr);
    }
    
};

#endif /* PhysicsComponent_hpp */
