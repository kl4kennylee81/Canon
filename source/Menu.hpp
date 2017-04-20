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
#include "UIComponent.hpp"

class Menu {
private:
    std::shared_ptr<cugl::Node> _menu;
	std::shared_ptr<cugl::Node> _menuBackground;
    std::vector<std::shared_ptr<UIComponent>> _uiElements;
public:
	std::shared_ptr<cugl::Node> getBackground();

	void setBackground(std::shared_ptr<cugl::Node> node);

    void addUIElement(std::shared_ptr<UIComponent> element);
    
    std::vector<std::shared_ptr<UIComponent>>& getUIElements();
    
    void attachToScene(std::shared_ptr<cugl::Node> parent);
    
    void detachFromScene();
    
    virtual bool init(bool touch);
    
    static std::shared_ptr<Menu> alloc(bool touch){
        std::shared_ptr<Menu> result = std::make_shared<Menu>();
        return (result->init(touch) ? result : nullptr);
    }

};

#endif /* Menu_hpp */
