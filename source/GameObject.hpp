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
    std::shared_ptr<PhysicsComponent> _body;
    std::shared_ptr<cugl::Node> _node;
    bool _isPlayer;
    
public:
    
    GameObject() :
    _body(nullptr),
    _node(nullptr){}
    
    bool init(std::shared_ptr<cugl::Node> node);
    
    bool init(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node);

    static std::shared_ptr<GameObject> alloc(std::shared_ptr<cugl::Node> node){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init(node) ? result : nullptr);
    }
    
    static std::shared_ptr<GameObject> alloc(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init(body,node) ? result : nullptr);
    }
    
    bool getIsPlayer() { return _isPlayer; };
    
    void setIsPlayer(bool val) { _isPlayer = val; };
    
    void setPhysicsComponent(std::shared_ptr<PhysicsComponent> body);
    
    /** get a weak reference to the Scene graph node, don't persist this reference 
     *  or we will have memory leaks */
    cugl::Node* getNode(){ return _node.get();}

    /** get a weak reference to the physics component, don't persist this reference 
     *  or we will have memory leaks */
    PhysicsComponent* getPhysicsComponent() {return _body.get();}
    
    void sync();
};

#endif /* GameObject_hpp */
