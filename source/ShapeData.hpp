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
    
    ShapeData(): Data(),height(0),width(0){};
    
    bool init(int uid,int h,int w){
        this->_uid = uid;
        this->height = h;
        this->width = w;
        return true;
    }
    
    static std::shared_ptr<ShapeData> alloc(int uid,int height,int width) {
        std::shared_ptr<ShapeData> result = std::make_shared<ShapeData>();
        return (result->init(uid,height,width) ? result : nullptr);
    }
    
    cugl::Size getSize(){
        return cugl::Size(width, height);
    }
    
    virtual std::string serialize();
    
    virtual bool preload(const std::string& file);
    
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json);
    
    virtual bool materialize();

};

#endif /* ShapeData_hpp */
