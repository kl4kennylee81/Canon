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
    this->_menu->addChild(element);
}

void Menu::attachToScene(){
    _scene->addChild(this->_menu);
}

void Menu::detachFromScene(){
    _scene->removeChild(this->_menu);
}

bool Menu::init(std::shared_ptr<cugl::Scene> scene, bool touch){
    this->_scene = scene;
    this->_menu = Node::alloc();
    return true;
}
