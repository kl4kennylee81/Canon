//
//  ObjectData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ObjectData_hpp
#define ObjectData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"
#include "Element.hpp"

class ObjectData : public Data {
public:
    // key uid to a path data preloaded into the world
    // used to create an activePath
//    int path_id;
    
    // used to spawn the physics component shape_id,speed,acceleration
    
    // key uid to a shape data preloaded into the world
    
    int shape_id;
    int animation_id;
    
    int speed;
    int acceleration;
    
    Element element;
    
    // key uid to animation/texture data preloaded into the world.
    // for example filmstrip texture will be passed
    // to reate the scene graph animationNode
    // and the information about different animation state
    // and how many frames to stay there
    // will be in the ActiveAnimation
    
//    int animation_id;
    
    ObjectData() : Data(){}
    
    bool init(int uid, int shape_id, int animation_id,int speed,int acceleration, Element element);
    
    static std::shared_ptr<ObjectData> alloc(int uid, int shape_id,int animation_id, int speed,int acceleration, Element element) {
        std::shared_ptr<ObjectData> result = std::make_shared<ObjectData>();
        return (result->init(uid,shape_id,animation_id,speed,acceleration,element) ? result : nullptr);
    }

    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
    
    Element getElement(){
        return element;
    }
};

#endif /* ObjectData_hpp */
