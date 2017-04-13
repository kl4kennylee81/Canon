//
//  Menu.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Menu.hpp"

using namespace cugl;


void Menu::addUIElement(std::shared_ptr<UIComponent> element){
    this->_uiElements.push_back(element);
    this->_menu->addChild(element->getNode());
}

std::vector<std::shared_ptr<UIComponent>>& Menu::getUIElements(){
    return _uiElements;
}

void Menu::attachToScene(std::shared_ptr<Node> parent){
    parent->addChild(this->_menu);
}

void Menu::detachFromScene(){
    this->_menu->removeFromParent();
}

bool Menu::init(bool touch){
    this->_menu = Node::alloc();
    return true;
}
