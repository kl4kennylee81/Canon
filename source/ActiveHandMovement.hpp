//
//  ActiveHandMovement.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/17/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActiveHandMovement_hpp
#define ActiveHandMovement_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "HandMovementComponent.hpp"
#include "ActivePath.hpp"

class ActiveHandMovement {
public:
    std::shared_ptr<HandMovementComponent> _component;
    std::shared_ptr<cugl::Node> _node; // texture for the hand node
    std::shared_ptr<ActivePath> _activePath; // active path for the handMovementPath
    
    ActiveHandMovement():
    _component(nullptr),
    _node(nullptr),
    _activePath(nullptr){}
    
    ~ActiveHandMovement(){ dispose(); };
    
    void dispose(){
        _component = nullptr;
        if (_node != nullptr){
            _node->removeFromParent();
        }
        _activePath = nullptr;
    }
    
    int _cooldownFrames;
    
    bool init(std::shared_ptr<GenericAssetManager> assets,
              std::shared_ptr<HandMovementComponent> c,
              std::map<std::string,std::string> fontMap);
    
    static std::shared_ptr<ActiveHandMovement> alloc(std::shared_ptr<GenericAssetManager> assets,
                                                     std::shared_ptr<HandMovementComponent> component,
                                                     std::map<std::string,std::string> fontMap) {
        std::shared_ptr<ActiveHandMovement> result = std::make_shared<ActiveHandMovement>();
        return (result->init(assets,component,fontMap) ? result : nullptr);
    };
    
    /** returns true if the active hand movement is done drawing */
    bool update();
    
    void reset();
};

#endif /* ActiveHandMovement_hpp */
