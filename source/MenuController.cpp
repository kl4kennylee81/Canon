//
//  MenuController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuController.hpp"
#include "InputController.hpp"
#include "UIData.hpp"
#include "MenuEvent.hpp"
#include "FinishEvent.hpp"
#include "LevelEditorEvent.hpp"

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
        case Event::EventType::FINISH:
        {
            FinishEvent* fEvent = (FinishEvent*)e;
            switch(fEvent->_finishType){
                case FinishEvent::FinishEventType::GAME_WON:
                {
                    this->getMenuGraph()->setActiveMenu("gameover");
                    break;
                }
                case FinishEvent::FinishEventType::GAME_LOST:
                {
                    this->getMenuGraph()->setActiveMenu("gameover");
                    break;
                }
            }
            break;
        }
        case Event::EventType::LEVEL_EDITOR:
        {
            LevelEditorEvent* levelEditorEvent = (LevelEditorEvent*) e;
            switch (levelEditorEvent->_levelEditType){
                case LevelEditorEvent::LevelEditorEventType::SIMULATE_LEVEL:
                {
                    _menuGraph->setNextMode(Mode::GAMEPLAY);
                    _menuGraph->setActiveMenu("gameScreen");
                    break;
                }
            }
            break;
        }
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
    if (!InputController::getIsPressedUp()){
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
        
        Vec2 vec = InputController::getPrevVector();
        
        // check if this button was clicked
        if (!button->containsScreen(vec)){
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
                        _selectedLevel = action->nextLevel;
						getMenuGraph()->setActiveMenu(action->nextScreen);
                        break;
                    }
                    case Mode::MAIN_MENU:
                    {
                        getMenuGraph()->setActiveMenu(action->nextScreen);
                        break;
                    }
                    case Mode::LEVEL_EDIT:
                    {
                        getMenuGraph()->setActiveMenu(action->nextScreen);
                        break;
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

				switch (action->fxKey) {
                    case FxTriggerButtonAction::FXType::PAUSE:
                    {
                        std::shared_ptr<Event> pauseEvent = PauseGameEvent::alloc(true);
						notify(pauseEvent.get());
                        getMenuGraph()->setActiveMenu(action->nextScreen);
						break;
                    }
					case FxTriggerButtonAction::FXType::RESUME:
                    {
                        std::shared_ptr<Event> pauseEvent = PauseGameEvent::alloc(false);
						notify(pauseEvent.get());
                        getMenuGraph()->setActiveMenu(action->nextScreen);
						break;
                    }
                    case FxTriggerButtonAction::FXType::RETRY:
                    {
                        std::shared_ptr<Event> resetEvent = ResetGameEvent::alloc();
                        notify(resetEvent.get());
                        getMenuGraph()->setActiveMenu(action->nextScreen);
                        break;
                    }
				}
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
