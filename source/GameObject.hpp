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
    
private:
    static int _atomicUidCounter;
    
protected:
    
    // HACK once we load from data files we want to uniquely identify a game object
    // with its uid. the uid would be set by the levelController when it spawns it
    int _uid;
    std::shared_ptr<PhysicsComponent> _body;
    std::shared_ptr<cugl::Node> _node;
    bool _isPlayer;
    
public:
    enum class ObjectType : int {
        CHARACTER,
        ZONE,
        BULLET
    };
    
    ObjectType type;
    
    GameObject() :
    _uid(0),
    _body(nullptr),
    _node(nullptr),
    _isPlayer(false){}
    
    ~GameObject(){ dispose(); }
    
    void dispose();
    
    bool init();
    
    bool init(std::shared_ptr<cugl::Node> node);
    
    bool init(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node);

    static std::shared_ptr<GameObject> alloc(){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init() ? result : nullptr);
    }
    
    static std::shared_ptr<GameObject> alloc(std::shared_ptr<cugl::Node> node){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init(node) ? result : nullptr);
    }
    
    static std::shared_ptr<GameObject> alloc(std::shared_ptr<PhysicsComponent> body,std::shared_ptr<cugl::Node> node){
        std::shared_ptr<GameObject> result = std::make_shared<GameObject>();
        return (result->init(body,node) ? result : nullptr);
    }
    
    bool getIsPlayer() { return _isPlayer; };
    
    int getUid() { return _uid; }
    
    void setIsPlayer(bool val) { _isPlayer = val; };
    
    void setUid(int uid) { _uid = uid; }
    
    void setPhysicsComponent(std::shared_ptr<PhysicsComponent> body);

    /** get a weak reference to the physics component, don't persist this reference 
     *  or we will have memory leaks */
    PhysicsComponent* getPhysicsComponent() {return _body.get();}
    
    /* Returns physics coordinates */
    cugl::Vec2 getPosition() {return _body->getBody()->getPosition();};

    void sync(float scale);
    
#pragma static methods
    
    static void resetAtomicUidCounter(){
        _atomicUidCounter = 0;
    }
    
    static int getAtomicUid(){
        _atomicUidCounter+=1;
        return _atomicUidCounter;
    }

};

#endif /* GameObject_hpp */
