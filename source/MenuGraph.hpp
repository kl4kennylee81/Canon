//
//  MenuGraph.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MenuGraph_hpp
#define MenuGraph_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Mode.hpp"
#include "Menu.hpp"
#include "MenuScreenData.hpp"
#include "GenericAssetManager.hpp"

class MenuGraph {
protected:
    Mode _currentMode;
    
    std::shared_ptr<Menu> _activeMenu;
    std::unordered_map<std::string, std::shared_ptr<Menu>> _menuMap;
    
public:
    MenuGraph();
    
    bool init(std::shared_ptr<cugl::Scene> scene,const std::shared_ptr<GenericAssetManager>& assets);
    
    static std::shared_ptr<MenuGraph> alloc(std::shared_ptr<cugl::Scene> scene,const std::shared_ptr<GenericAssetManager>& assets) {
        std::shared_ptr<MenuGraph> result = std::make_shared<MenuGraph>();
        return (result->init(scene,assets) ? result : nullptr);
    }
    
    void setActiveMenu(std::shared_ptr<Menu> menu);
    
    void setMode(Mode mode);
    
    Mode getMode();
};

#endif /* MenuGraph_hpp */
