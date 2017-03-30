//
//  Menu.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Menu.hpp"

using namespace cugl;


void Menu::addUIElement(std::shared_ptr<cugl::Node> element){
    this->_uiElements.push_back(element);
}

void Menu::attachToScene(){
    _scene->addChild(this->_menu);
    
    for (auto element : _uiElements){
        this->_menu->addChild(element);
    }
}

void Menu::detachFromScene(){
    for (auto element : _uiElements){
        this->_menu->removeChild(element);
    }
    _scene->removeChild(this->_menu);
}

bool Menu::init(std::shared_ptr<cugl::Scene> scene, bool touch){
    this->_scene = scene;
    this->_menu = Node::alloc();
    return true;
}
