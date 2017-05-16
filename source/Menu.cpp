//
//  Menu.cpp
//  Canon
//
//  Created by Kenneth Lee on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Menu.hpp"
#include "GameState.hpp"
#include "Util.hpp"

using namespace cugl;

std::shared_ptr<cugl::Node> Menu::getBackground() { return _menuBackground; }

std::string Menu::getMenuKey() { return _mKey; }

void Menu::setBackground(std::shared_ptr<cugl::Node> node)
{ 
	_menuBackground = node;
	this->_menu->addChild(node, 0);
}

void Menu::addUIElement(std::shared_ptr<UIComponent> element){
    this->_uiElements.push_back(element);
    this->_menu->addChild(element->getNode(),5);
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

bool Menu::init(std::string mKey){
    this->_menu = Node::alloc();
	this->_mKey = mKey;
    return true;
}

void Menu::populate(std::shared_ptr<GenericAssetManager> assets, std::vector<std::string> uiDataKeys,std::string bgKey,std::map<std::string,std::string> fontMap){
    // make sure the fontmap is set
    this->setFontMap(fontMap);
    
    if (bgKey != "") {
        // texture fetch and scale: note, we put this before uielements because z-orders are not automatically enforced..it's by order actually
        std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(bgKey));
        cugl::Size imageSize = imageNode->getSize();
        imageNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        imageNode->setScale(Vec2(GAME_SCENE_WIDTH / imageSize.width, Util::getGameSceneHeight() / imageSize.height));
        imageNode->setPosition(Vec2::ZERO);
        
        this->setBackground(imageNode);
    }
    
    for (std::string uiKey : uiDataKeys) {
        auto uiData = assets->get<UIData>(uiKey);
        std::shared_ptr<Node> uiNode = uiData->dataToNode(assets,fontMap);
        std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData, uiNode);
        this->addUIElement(uiComponent);
        
    }
}
