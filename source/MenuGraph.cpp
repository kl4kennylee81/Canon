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
#include "SaveGameData.hpp"

#define SAVE_GAME_FILE "saveFile"

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

std::shared_ptr<Menu> createLevelMenu(const std::shared_ptr<GenericAssetManager>& assets){
    std::shared_ptr<SaveGameData> saveGame = assets->get<SaveGameData>(SAVE_GAME_FILE);
    std::shared_ptr<Menu> menu = Menu::alloc(false);
    int i = 0;
    for(auto entry : saveGame->getSaveLevelEntries()){
        std::shared_ptr<ButtonAction> action = ModeChangeButtonAction::alloc(Mode::GAMEPLAY,entry->levelKey);
        
        // TODO hacky setting of the uiKey
        // TODO create a template for the level entry button
        std::shared_ptr<ButtonUIData> button = ButtonUIData::alloc("entry"+std::to_string(i+1),"play",512,400-50*i,100,100,action,"");
        std::shared_ptr<Node> buttonNode = button->dataToNode(assets);
        std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(button,buttonNode);
        menu->addUIElement(uiComponent);
    }
    return menu;
};

void MenuGraph::populate(const std::shared_ptr<GenericAssetManager>& assets){
    
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
    
    std::shared_ptr<Menu> levelMenu = createLevelMenu(assets);
    _menuMap.insert(std::make_pair("levelSelect",levelMenu));
    
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
