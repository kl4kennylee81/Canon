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
    std::shared_ptr<cugl::Scene> _scene;
    std::shared_ptr<cugl::Node> _menu;
    
    std::vector<std::shared_ptr<cugl::Node>> _uiElements;
public:
    
    void addUIElement(std::shared_ptr<cugl::Node> element);
    
    void attachToScene();
    
    void detachFromScene();
    
    virtual bool init(std::shared_ptr<cugl::Scene> scene, bool touch);
    
    static std::shared_ptr<Menu> alloc(std::shared_ptr<cugl::Scene> scene, bool touch){
        std::shared_ptr<Menu> result = std::make_shared<Menu>();
        return (result->init(scene,touch) ? result : nullptr);
    }

};

#endif /* Menu_hpp */
