//
//  MenuGraph.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuGraph.hpp"
#include "MenuScreenData.hpp"
#include "UIComponent.hpp"

using namespace cugl;

MenuGraph::MenuGraph(){}

bool MenuGraph::init(){
    _currentMode = Mode::LOADING;
    _nextMode = Mode::LOADING;
    _menuNode = Node::alloc();
    _activeMenu = nullptr;
    return true;
}

bool MenuGraph::init(const std::shared_ptr<GenericAssetManager>& assets){
    init();
    // TODO prepopulate the menu graph
    this->populate(assets);
    return true;
}

void MenuGraph::populate(const std::shared_ptr<GenericAssetManager>& assets){
    Size size = Application::get()->getDisplaySize();
    
    std::shared_ptr<MenuScreenData> menuScreens = assets->get<MenuScreenData>("menus");
    
    for (auto entry : menuScreens->getMenuEntries() ){
        std::string menuKey = entry.first;
        std::shared_ptr<MenuEntry> menuEntry = entry.second;
        std::shared_ptr<Menu> menu = Menu::alloc(false);
        
        for (std::string uiKey : menuEntry->getUIEntryKeys()){
            auto uiData = assets->get<UIData>(uiKey);
            std::shared_ptr<Node> uiNode = uiData->dataToNode(assets);
            std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData,uiNode);
            menu->addUIElement(uiComponent);
        }
        _menuMap.insert(std::make_pair(menuEntry->menuKey,menu));
    }
}

void MenuGraph::setActiveMenu(std::shared_ptr<Menu> menu){
    if (_activeMenu != nullptr){
        // detach old screen from scene
        _activeMenu->detachFromScene();
    }
    _activeMenu = menu;
    
    if (menu == nullptr){
        return;
    }

    // add new screen to scene
    _activeMenu->attachToScene(_menuNode);
}

void MenuGraph::setMode(Mode mode){
    switch (mode){
        case Mode::LOADING:
        {
            break;
        }
        case Mode::GAMEPLAY:
        {
            break;
        }
        case Mode::MAIN_MENU:
        {
            // TODO this is a little hacky rn to switch back to the main menu
            this->setActiveMenu(_menuMap.at("startMenu"));
            break;
        }
        default:
        {
            break;
        }
    }
    _currentMode = mode;
}

void MenuGraph::setNextMode(Mode mode){
    _nextMode = mode;
}

void MenuGraph::setActiveMenu(std::string nextTarget){
    std::shared_ptr<Menu> targetMenu = _menuMap.at(nextTarget);
    if (targetMenu == nullptr){
        return;
    }
    setActiveMenu(targetMenu);
}

void MenuGraph::updateToNextMode(){
    setMode(_nextMode);
}

Mode MenuGraph::getMode(){
    return _currentMode;
}

Mode MenuGraph::getNextMode(){
    return _nextMode;
}

std::shared_ptr<Menu>& MenuGraph::getActiveMenu(){
    return _activeMenu;
}

bool MenuGraph::needsUpdate(){
    return _currentMode != _nextMode;
}

void MenuGraph::attachToScene(std::shared_ptr<cugl::Scene> scene){
    scene->addChild(_menuNode);
}

void MenuGraph::detachFromScene(std::shared_ptr<cugl::Scene> scene){
    scene->removeChild(_menuNode);
}
