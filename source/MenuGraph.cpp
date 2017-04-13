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
    
    std::shared_ptr<Menu> mainMenu = Menu::alloc(false);
    
    std::shared_ptr<Label> label1 = Label::alloc("main menu", assets->get<Font>("Charlemagne"));
    label1->setPosition(Vec2(size.width/2.0f,400));
    mainMenu->addUIElement(label1);
    
    auto play = PolygonNode::allocWithTexture(assets->get<Texture>("play"));
    std::shared_ptr<Button> button1 = Button::alloc(play);
    button1->setAnchor(Vec2::ANCHOR_MIDDLE);
    button1->setPosition(Vec2(size.width/2.0f,200));
    button1->setListener([=](const std::string& name, bool down) {
        if (down){
            return;
        }
        setActiveMenu(this->_menuMap.at("levelMenu"));
    });
    button1->setVisible(true);
    button1->activate(1);
    mainMenu->addUIElement(button1);
    
    _menuMap.insert(std::make_pair("mainMenu",mainMenu));
    
    std::shared_ptr<Menu> levelMenu = Menu::alloc(false);
    
    std::shared_ptr<Label> label2 = Label::alloc("level menu", assets->get<Font>("Charlemagne"));
    label2->setPosition(Vec2(size.width/2.0f,400));
    
    levelMenu->addUIElement(label2);
    
    play = PolygonNode::allocWithTexture(assets->get<Texture>("play"));
    std::shared_ptr<Button> button2 = Button::alloc(play);
    button2->setAnchor(Vec2::ANCHOR_MIDDLE);
    button2->setPosition(Vec2(size.width/2.0f,300));
    button2->setListener([=](const std::string& name, bool down) {
        if (down){
            return;
        }
        setNextMode(Mode::GAMEPLAY);
    });
    button2->setVisible(true);
    button2->activate(2);
    levelMenu->addUIElement(button2);
    
    _menuMap.insert(std::make_pair("levelMenu",levelMenu));
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
            this->setActiveMenu(_menuMap.at("levelMenu"));
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

void MenuGraph::updateToNextMode(){
    setMode(_nextMode);
}

Mode MenuGraph::getMode(){
    return _currentMode;
}

Mode MenuGraph::getNextMode(){
    return _nextMode;
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
