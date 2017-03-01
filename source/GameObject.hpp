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
    
public:
    
    GameObject() :
    _body(nullptr),
    _node(nullptr){}
    
    bool init(std::unique_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node);
    
    static std::shared_ptr<GameObject> alloc(std::unique_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init(std::move(body),node) ? result : nullptr);
    }
};

#endif /* GameObject_hpp */
