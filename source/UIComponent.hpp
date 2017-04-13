//
//  UIComponent.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/13/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef UIComponent_hpp
#define UIComponent_hpp

#include <stdio.h>
#include "UIData.hpp"

class UIComponent {
private:
    std::shared_ptr<UIData> _uiData; // stores the static info needed for the uiComponent
    std::shared_ptr<cugl::Node> _node; // the active on screen button/label/image scene graph node
public:
    
    std::shared_ptr<UIData> getUIData(){
        return _uiData;
    }
    
    std::shared_ptr<cugl::Node> getNode(){
        return _node;
    }
    
    bool init(std::shared_ptr<UIData> uiData,std::shared_ptr<cugl::Node> node){
        _uiData = uiData;
        _node = node;
        return true;
    }
    
    static std::shared_ptr<UIComponent> alloc(std::shared_ptr<UIData> uiData,std::shared_ptr<cugl::Node> node){
        std::shared_ptr<UIComponent> result = std::make_shared<UIComponent>();
        return (result->init(uiData,node) ? result : nullptr);
    }
    
};

#endif /* UIComponent_hpp */
