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

class ObjectData : public Data {
public:
    // key uid to a path data preloaded into the world
    // used to create an activePath
//    int path_id;
    
    // used to spawn the physics component shape_id,speed,acceleration
    
    // key uid to a shape data preloaded into the world
    int shape_id;
    
    int speed;
    int acceleration;
    
    // key uid to animation/texture data preloaded into the world.
    // for example filmstrip texture will be passed
    // to reate the scene graph animationNode
    // and the information about different animation state
    // and how many frames to stay there
    // will be in the ActiveAnimation
    
//    int animation_id;
    
    ObjectData() : Data(){}
    
    ObjectData(int uid) : Data(uid){}
    
    bool init(){ return true; }
    
    bool init(int uid, int shape_id, int speed,int acceleration);
    
    static std::shared_ptr<ObjectData> alloc() {
        std::shared_ptr<ObjectData> result = std::make_shared<ObjectData>();
        return (result->init() ? result : nullptr);
    }

    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();
};

#endif /* ObjectData_hpp */
