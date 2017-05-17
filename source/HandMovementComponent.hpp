//
//  HandMovementComponent.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef HandMovementComponent_hpp
#define HandMovementComponent_hpp

#include <stdio.h>
#include "cugl/cugl.h"
#include "Path.hpp"

class HandMovementComponent {
public:
    std::shared_ptr<cugl::Texture> texture;
    
    std::shared_ptr<Path> path;
    
    bool repeat;
    
    float speed; // magnitude of the speed of travel for scene cordinates
    
    bool init(std::shared_ptr<cugl::JsonValue> json);
    
    std::shared_ptr<HandMovementComponent> alloc(std::shared_ptr<cugl::JsonValue> json){
        std::shared_ptr<HandMovementComponent> result = std::make_shared<HandMovementComponent>();
        return (result->init(json) ? result : nullptr);
    }
    
};

#endif /* HandMovementComponent_hpp */
