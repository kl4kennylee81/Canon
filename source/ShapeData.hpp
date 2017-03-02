//
//  ShapeData.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ShapeData_hpp
#define ShapeData_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Data.hpp"

class ShapeData : public Data {
public:
    int height;
    int width;
    
    ShapeData(int uid): Data(uid),height(0),width(0){};
    
    ShapeData(int uid,int height,int width):Data(uid){
        height = height;
        width = width;
    };
    
    bool init(){
        return true;
    }
    
    static std::shared_ptr<ShapeData> alloc(int uid,int height,int width) {
        std::shared_ptr<ShapeData> result = std::make_shared<ShapeData>(uid,height,width);
        return (result->init() ? result : nullptr);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();

};

#endif /* ShapeData_hpp */
