//
//  Menu.hpp
//  Canon
//
//  Created by Kenneth Lee on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class Menu {
private:
    std::shared_ptr<cugl::Node> _menu;
    
    std::vector<std::shared_ptr<cugl::Node>> _uiElements;
public:
    
    void addUIElement(std::shared_ptr<cugl::Node> element);
    
    std::vector<std::shared_ptr<cugl::Node>>& getUIElements();
    
    void attachToScene(std::shared_ptr<cugl::Node> parent);
    
    void detachFromScene();
    
    virtual bool init(bool touch);
    
    static std::shared_ptr<Menu> alloc(bool touch){
        std::shared_ptr<Menu> result = std::make_shared<Menu>();
        return (result->init(touch) ? result : nullptr);
    }

};

#endif /* Menu_hpp */
