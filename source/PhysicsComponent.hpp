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
    std::shared_ptr<cugl::PolygonObstacle> _body;
    float _speed;
    float _acceleration;
    ElementType _elementType;
public:
    
    PhysicsComponent();
    
    ~PhysicsComponent(){ dispose(); }
    
    void dispose();
    
    virtual bool init(std::shared_ptr<cugl::PolygonObstacle> body, ElementType element);
    
    static std::shared_ptr<PhysicsComponent> alloc(std::shared_ptr<cugl::PolygonObstacle> body, ElementType element) {
        std::shared_ptr<PhysicsComponent> result = std::make_shared<PhysicsComponent>();
        return (result->init(body, element) ? result : nullptr);
    }
    std::shared_ptr<cugl::PolygonObstacle> getBody();
    
    ElementType getElementType() { return _elementType; };
    
    void setBody(std::shared_ptr<cugl::PolygonObstacle> body);
    
};

#endif /* PhysicsComponent_hpp */
