//
//  HandMovementComponent.cpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "HandMovementComponent.hpp"

using namespace cugl;

bool HandMovementComponent::init(std::shared_ptr<cugl::JsonValue> json){
    texture = Texture::allocWithFile(json->getString("texture"));
    
    
    
//    
//    std::shared_ptr<cugl::Texture> texture;
//    
//    std::shared_ptr<Path> path;
//    
//    bool repeat;
//    
//    float speed; // magnitude of the speed of travel for scene cordinates
    
    
    
    
    
    return true;
}
