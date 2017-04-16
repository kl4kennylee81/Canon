//
//  MenuController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuController.hpp"
#include "InputController.hpp"

using namespace cugl;

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

MenuController::MenuController() :
BaseController(),
_menuGraph(nullptr){}

void MenuController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void MenuController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void MenuController::notify(Event* e) {
    BaseController::notify(e);
}

void MenuController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        default:
            break;
    }
}

void MenuController::update(float timestep) {
    
    std::shared_ptr<Menu> activeMenu = this->getMenuGraph()->getActiveMenu();
    if (activeMenu == nullptr){
        return;
    }
    
    // check that a press has been made
    if (!InputController::getIsPressed()){
        return;
    }
    
    for (auto uiElement : activeMenu->getUIElements()){
        // check has an action
        if (uiElement->getAction() == nullptr){
            continue;
        }
        
        // check if the uiElement is a button
        if (std::dynamic_pointer_cast<Button>(uiElement->getNode()) == nullptr){
            continue;
        }
        
        std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(uiElement->getNode());
        
        // check if this button was clicked
        if (!button->containsScreen(InputController::getInputVector())){
            continue;
        }
        
        // execute the appropriate action
        switch(uiElement->getAction()->type){
            case ButtonActionType::MENUCHANGE:
            {
                std::shared_ptr<MenuChangeButtonAction> action = std::dynamic_pointer_cast<MenuChangeButtonAction>(uiElement->getAction());
                getMenuGraph()->setActiveMenu(action->menuTarget);
                break;
            }
            case ButtonActionType::MODECHANGE:
            {
                std::shared_ptr<ModeChangeButtonAction> action = std::dynamic_pointer_cast<ModeChangeButtonAction>(uiElement->getAction());
                getMenuGraph()->setNextMode(action->modeTarget);
                switch(action->modeTarget){
                    case Mode::GAMEPLAY:
                    {
                        _selectedLevel = action->nextScreen;
                        break;
                    }
                    case Mode::MAIN_MENU:
                    {
                        getMenuGraph()->setActiveMenu(action->nextScreen);
                    }
                    default:
                    {
                        break;
                    }
                }
                
                break;
            }
            case ButtonActionType::FXTRIGGER:
            {
                std::shared_ptr<FxTriggerButtonAction> action = std::dynamic_pointer_cast<FxTriggerButtonAction>(uiElement->getAction());
                break;
            }
        }
    }
    
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void MenuController::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    this->_scene->render(_batch);
}

bool MenuController::init(std::shared_ptr<MenuGraph> menuGraph){
    _menuGraph = menuGraph;
    _selectedLevel = "";
    return true;
}

bool MenuController::init(std::shared_ptr<cugl::Scene> scene,
                          std::shared_ptr<MenuGraph> menuGraph){
    _scene = scene;
    _menuGraph = menuGraph;
    _selectedLevel = "";
    this->activate();
    return true;
}

void MenuController::dispose(){
    deactivate();
    _scene = nullptr;
    _menuGraph = nullptr;
}

void MenuController::activate(){
    _menuGraph->attachToScene(_scene);
}

void MenuController::deactivate(){
    _menuGraph->detachFromScene(_scene);
}

std::string MenuController::getSelectedLevel(){
    return _selectedLevel;
}
