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
    std::shared_ptr<ButtonAction> _action; // stores the static info ONLY for buttons otherwise nullptr
    std::shared_ptr<cugl::Node> _node; // the active on screen button/label/image scene graph node
public:
    
    std::shared_ptr<ButtonAction> getAction(){
        return _action;
    }
    
    std::shared_ptr<cugl::Node> getNode(){
        return _node;
    }
    
    bool init(std::shared_ptr<UIData> uiData,std::shared_ptr<cugl::Node> node){
        _node = node;
        _action = nullptr;
        if (uiData->type == UIDataType::BUTTON){
            std::shared_ptr<ButtonUIData> buttonData = std::dynamic_pointer_cast<ButtonUIData>(uiData);
            _action = buttonData->buttonAction;
        }
        return true;
    }
    
    static std::shared_ptr<UIComponent> alloc(std::shared_ptr<UIData> uiData,std::shared_ptr<cugl::Node> node){
        std::shared_ptr<UIComponent> result = std::make_shared<UIComponent>();
        return (result->init(uiData,node) ? result : nullptr);
    }
    
};

#endif /* UIComponent_hpp */
