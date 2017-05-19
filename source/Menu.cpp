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

#define STORY_SLIDE 8

using namespace cugl;

std::shared_ptr<ActiveAnimation> Menu::getBackground() { return _menuBackground; }

std::string Menu::getMenuKey() { return _mKey; }

void Menu::setBackground(std::shared_ptr<ActiveAnimation> anim)
{
	_menuBackground = anim;
	this->_menu->addChild(anim->getNode(), 0);
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

void Menu::updateAnimation(){
    if (_menuBackground != nullptr) {
        _menuBackground->nextFrame();
    }
    if (_storyScreen != nullptr && _storyScreen != nullptr) {
        _storyScreen->setPosition(_storyScreen->getPositionX()-STORY_SLIDE,0);
        
        if (_storyScreen->getPositionX() + _storyScreen->getWidth() < 0) {
            _storyScreen->setPosition(_storyScreen->getPositionX()+_storyScreen->getWidth()*2,0);
        }
        
        _storyScreen2->setPosition(_storyScreen2->getPositionX()-STORY_SLIDE,0);
        
        if (_storyScreen2->getPositionX() + _storyScreen2->getWidth() < 0) {
            _storyScreen2->setPosition(_storyScreen2->getPositionX()+_storyScreen2->getWidth()*2,0);
        }
        
    }
}

void Menu::populate(std::shared_ptr<GenericAssetManager> assets, std::vector<std::string> uiDataKeys,std::string bgKey,std::map<std::string,std::string> fontMap){
    // make sure the fontmap is set
    this->setFontMap(fontMap);
    
    if (bgKey != "") {
        // texture fetch and scale: note, we put this before uielements because z-orders are not automatically enforced..it's by order actually
        /*
        std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(bgKey));
        cugl::Size imageSize = imageNode->getSize();
        imageNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        imageNode->setScale(Vec2(GAME_SCENE_WIDTH / imageSize.width, Util::getGameSceneHeight() / imageSize.height));
        imageNode->setPosition(Vec2::ZERO);
        
        this->setBackground(imageNode);*/
        
        std::shared_ptr<AnimationData> animData = assets->get<AnimationData>(bgKey);
        std::shared_ptr<ActiveAnimation> anim = ActiveAnimation::alloc();
        anim->setAnimationData(animData);
        anim->handleAction(AnimationAction::ACTIVE);
        
        std::shared_ptr<cugl::AnimationNode> animNode = anim->getNode();
        cugl::Size imageSize = animNode->getBoundingRect().size;
        animNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        animNode->setScale(Vec2(GAME_SCENE_WIDTH / imageSize.width, Util::getGameSceneHeight() / imageSize.height));
        animNode->setPosition(Vec2::ZERO);
        
        this->setBackground(anim);
    }
    
    for (std::string uiKey : uiDataKeys) {
        if (uiKey == "storyScreen_storyImage") {
            auto uiData = assets->get<UIData>(uiKey);
            auto storyImage = assets->get<Texture>(uiData->uiBackgroundKey);
            std::shared_ptr<Node> uiNode = uiData->dataToNode(assets,fontMap);
            _storyScreen = std::static_pointer_cast<PolygonNode>(uiNode);
        
            _storyScreen->setScale(Util::getGameSceneHeight()/storyImage->getHeight());
            _storyScreen->setPosition(0,0);
            std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData, _storyScreen);
            this->addUIElement(uiComponent);
        } else if (uiKey == "storyScreen_storyImage2") {
            auto uiData = assets->get<UIData>(uiKey);
            auto storyImage = assets->get<Texture>(uiData->uiBackgroundKey);
            std::shared_ptr<Node> uiNode = uiData->dataToNode(assets,fontMap);
            _storyScreen2 = std::static_pointer_cast<PolygonNode>(uiNode);
            
            _storyScreen2->setScale(Util::getGameSceneHeight()/storyImage->getHeight());
            _storyScreen2->setPosition(_storyScreen2->getWidth(),0);
            std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData, _storyScreen2);
            this->addUIElement(uiComponent);
            
        } else {
            auto uiData = assets->get<UIData>(uiKey);
            std::shared_ptr<Node> uiNode = uiData->dataToNode(assets,fontMap);
            std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData, uiNode);
            this->addUIElement(uiComponent);
        }
        
    }
}
