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
#include "GameState.hpp"
#include "SaveGameData.hpp"
#include "Util.hpp"
#include "MenuListData.hpp"

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

void augmentLevelMenu(const std::shared_ptr<GenericAssetManager>& assets, const std::unordered_map<std::string, std::shared_ptr<Menu>> map)
{
	std::shared_ptr<SaveGameData> saveGame = assets->get<SaveGameData>(SAVE_GAME_FILE);
	// TODO replace the hardcoded name
	std::shared_ptr<Menu> selectMenu = map.at("levelSelect");

	int i = 0;
	for (auto entry : saveGame->getSaveLevelEntries())
	{
		std::shared_ptr<ButtonAction> action = ModeChangeButtonAction::alloc(Mode::GAMEPLAY, "gameScreen",entry->levelKey);
		// TODO hacky setting of the uiKey
		std::shared_ptr<UIData> boxData = assets->get<UIData>("levelSelectStarCircle");
        float x = 0.1 + (0.225*(i%4));
        float y = 0.45 - (0.26 * (i/4));
		std::shared_ptr<ButtonUIData> button = ButtonUIData::alloc("entry" + std::to_string(i + 1), "levelSelectStarCircle", x, y, boxData->width, boxData->height, action, "");
		std::shared_ptr<Node> buttonNode = button->dataToNode(assets);

		// make label for level entry
		std::shared_ptr<UIData> labelText = assets->get<UIData>("levelLabelText");
		std::shared_ptr<TextUIData> textData = std::dynamic_pointer_cast<TextUIData>(labelText);
		textData->textValue = entry->name;
		std::shared_ptr<Node> labelNode = textData->dataToNode(assets);
        labelNode->setPosition(x+225,y+300);
		buttonNode->addChild(labelNode, 3);

		//std::shared_ptr<UIComponent> labelComponent = UIComponent::alloc(labelText, labelNode);
		std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(button, buttonNode);
		selectMenu->addUIElement(uiComponent);
		//selectMenu->addUIElement(labelComponent);
		i++;
	}
}

void MenuGraph::populate(const std::shared_ptr<GenericAssetManager>& assets){
	std::shared_ptr<MenuListData> menuList = assets->get<MenuListData>("menuList");

	for (std::string key : menuList->getMenuKeys()) {
		// hack: remove this later
		if (key == "randomMenuAssets") {
			continue;
		}
		std::shared_ptr<MenuScreenData> menuData = assets->get<MenuScreenData>(key);
        std::string menuKey = menuData->key;
		std::string menuBackgroundKey = menuData->menuBackgroundKey;
		std::shared_ptr<Menu> menu = Menu::alloc(false,menuKey);

		if (menuBackgroundKey != "") {
			// texture fetch and scale: note, we put this before uielements because z-orders are not automatically enforced..it's by order actually
			std::shared_ptr<Node> imageNode = PolygonNode::allocWithTexture(assets->get<Texture>(menuBackgroundKey));
			cugl::Size imageSize = imageNode->getSize();
			imageNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
			imageNode->setScale(Vec2(GAME_SCENE_WIDTH / imageSize.width, Util::getGameSceneHeight() / imageSize.height));
			imageNode->setPosition(Vec2::ZERO);

			menu->setBackground(imageNode);
		}

		for (std::string uiKey : menuData->getUIEntryKeys()) {
			auto uiData = assets->get<UIData>(uiKey);
			std::shared_ptr<Node> uiNode = uiData->dataToNode(assets);
			std::shared_ptr<UIComponent> uiComponent = UIComponent::alloc(uiData, uiNode);
			menu->addUIElement(uiComponent);

		}

		_menuMap.insert(std::make_pair(menuKey, menu));
	}

	augmentLevelMenu(assets, _menuMap);
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
            this->setActiveMenu(_menuMap.at("levelSelect"));
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

void MenuGraph::attachToScene(std::shared_ptr<Scene> scene){
    scene->addChildWithName(_menuNode,"menuNode",3);
}

void MenuGraph::detachFromScene(std::shared_ptr<Scene> scene){
    if (scene->getChildByName("menuNode") == nullptr){
        return;
    }
    scene->removeChild(_menuNode);
}

std::shared_ptr<JsonValue> MenuGraph::toJsonValue(){
	std::shared_ptr<JsonValue> mg = JsonValue::allocObject();
	mg->appendChild("currentMode", JsonValue::alloc(modeToString(_currentMode)));
	mg->appendChild("nextMode", JsonValue::alloc(modeToString(_nextMode)));
	mg->appendChild("activeMenu", JsonValue::alloc(_activeMenu->getMenuKey()));
	return mg;
}

bool MenuGraph::initAfterResume(std::shared_ptr<JsonValue> menuGraphResumeJson){
    _currentMode = stringToMode(menuGraphResumeJson->getString("currentMode"));
    _nextMode = stringToMode(menuGraphResumeJson->getString("nextMode"));
    
    _activeMenu = _menuMap.at(menuGraphResumeJson->getString("activeMenu"));
    if (_currentMode == Mode::GAMEPLAY){
        // hardcoded to go pause the screen when in gameplay state
        setActiveMenu("pauseScreen");
    }
    return true;
}
