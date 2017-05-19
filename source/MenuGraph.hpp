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
    std::shared_ptr<cugl::Node> _menuNode;
    
    Mode _currentMode;
    Mode _nextMode;
    std::shared_ptr<Menu> _activeMenu;
    
    std::unordered_map<std::string, std::shared_ptr<Menu>> _menuMap;
    
    void setMode(Mode mode);
    
    std::string _currentChapter;
public:
    MenuGraph();
    
    bool init();
    
    bool init(const std::shared_ptr<GenericAssetManager>& assets);
    
    static std::shared_ptr<MenuGraph> alloc() {
        std::shared_ptr<MenuGraph> result = std::make_shared<MenuGraph>();
        return (result->init() ? result : nullptr);
    }
    
    static std::shared_ptr<MenuGraph> alloc(const std::shared_ptr<GenericAssetManager>& assets) {
        std::shared_ptr<MenuGraph> result = std::make_shared<MenuGraph>();
        return (result->init(assets) ? result : nullptr);
    }

	std::unordered_map<std::string, std::shared_ptr<Menu>> getMenuMap() { return _menuMap; }
    
	void populateGameplayMenu(const std::shared_ptr<GenericAssetManager>& assets, const std::unordered_map<std::string, std::shared_ptr<Menu>>& map, int chIndex, int lvlIndex);
	void augmentLevelMenu(const std::shared_ptr<GenericAssetManager>& assets, const std::unordered_map<std::string, std::shared_ptr<Menu>> map, std::string chapter);

	void populateChapter(const std::shared_ptr<GenericAssetManager>& assets, std::string chapter);
    void populate(const std::shared_ptr<GenericAssetManager>& assets);
    
    void setActiveMenu(std::shared_ptr<Menu> menu);
    
    void setActiveMenu(std::string nextTarget);
    
    void setNextMode(Mode mode);
    
    void updateToNextMode();
    
    Mode getMode();
    
    Mode getNextMode();
    
    std::shared_ptr<Menu>& getActiveMenu();
    
    bool needsUpdate();
    
    void attachToScene(std::shared_ptr<cugl::Scene> scene);
    
    void detachFromScene(std::shared_ptr<cugl::Scene> scene);
    
	std::shared_ptr<cugl::JsonValue> toJsonValue();
    
    bool initAfterResume(std::shared_ptr<cugl::JsonValue> menuGraphResumeJson);
    
    std::string getCurrentChapter() { return _currentChapter;}
};

#endif /* MenuGraph_hpp */
