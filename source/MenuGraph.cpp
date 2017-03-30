//
//  MenuGraph.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuGraph.hpp"

using namespace cugl;

MenuGraph::MenuGraph(){}

bool MenuGraph::init(){
    _currentMode = Mode::LOADING;
    _activeMenu = nullptr;
    return true;
}

bool MenuGraph::init(std::shared_ptr<Scene> scene,const std::shared_ptr<GenericAssetManager>& assets){
    init();
    
    // TODO prepopulate the menu graph
    
    Size size = Application::get()->getDisplaySize();
    
    std::shared_ptr<Menu> mainMenu = Menu::alloc(scene,false);
    
    std::shared_ptr<Label> label1 = Label::alloc("main menu", assets->get<Font>("Charlemagne"));
    label1->setPosition(Vec2(size.width/2.0f,400));
    mainMenu->addUIElement(label1);
    
    auto play = PolygonNode::allocWithTexture(assets->get<Texture>("play"));
    std::shared_ptr<Button> button1 = Button::alloc(play);
    button1->setAnchor(Vec2::ANCHOR_MIDDLE);
    button1->setPosition(Vec2(size.width/2.0f,200));
    button1->setListener([=](const std::string& name, bool down) {
        setActiveMenu(this->_menuMap.at("levelMenu"));
    });
    button1->setVisible(true);
    mainMenu->addUIElement(button1);
    
    _menuMap.insert(std::make_pair("mainMenu",mainMenu));
    
    std::shared_ptr<Menu> levelMenu = Menu::alloc(scene,false);
    
    std::shared_ptr<Label> label2 = Label::alloc("level menu", assets->get<Font>("Charlemagne"));
    label2->setPosition(Vec2(size.width/2.0f,400));
    
    levelMenu->addUIElement(label2);
    
    play = PolygonNode::allocWithTexture(assets->get<Texture>("play"));
    std::shared_ptr<Button> button2 = Button::alloc(play);
    button2->setAnchor(Vec2::ANCHOR_MIDDLE);
    button2->setPosition(Vec2(size.width/2.0f,200));
    button2->setListener([=](const std::string& name, bool down) {
        setActiveMenu(this->_menuMap.at("mainMenu"));
    });
    button2->setVisible(true);
    mainMenu->addUIElement(button2);
    
    levelMenu->addUIElement(button2);
    
    _menuMap.insert(std::make_pair("levelMenu",levelMenu));
    
    return true;
}

void MenuGraph::setActiveMenu(std::shared_ptr<Menu> menu){
    if (_activeMenu != nullptr){
        // detach old screen from scene
        _activeMenu->detachFromScene();
    }
    _activeMenu = menu;

    // add new screen to scene
    _activeMenu->attachToScene();
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
            this->setActiveMenu(_menuMap.at("mainMenu"));
            break;
        }
        default:
        {
            break;
        }
    }
    _currentMode = mode;
}

Mode MenuGraph::getMode(){
    return _currentMode;
}
