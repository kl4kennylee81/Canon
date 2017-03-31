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
    Element _elementType;
public:
    
    PhysicsComponent();
    
    ~PhysicsComponent(){ dispose(); }
    
    void dispose();
    
    virtual bool init(std::shared_ptr<cugl::PolygonObstacle> body, Element element);
    
    static std::shared_ptr<PhysicsComponent> alloc(std::shared_ptr<cugl::PolygonObstacle> body, Element element) {
        std::shared_ptr<PhysicsComponent> result = std::make_shared<PhysicsComponent>();
        return (result->init(body, element) ? result : nullptr);
    }
    std::shared_ptr<cugl::PolygonObstacle> getBody();
    
    Element getElementType() { return _elementType; };
    
};

#endif /* PhysicsComponent_hpp */
