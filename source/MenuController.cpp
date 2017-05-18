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
#include "LevelSelectData.hpp"

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
                    this->getMenuGraph()->setActiveMenu("victory");
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
    
    // check that a press has been made or in the past
    // previously pressed is used because when pressing up it is no longer currently pressed
    if (!InputController::getIsPressedUp() && !InputController::getIsPressed()){
        return;
    }
    
    for (auto uiElement : activeMenu->getUIElements()){
        // check has an action
        if (uiElement->getAction() == nullptr){
            continue;
        }
        
        std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(uiElement->getNode());
        
        if (button == nullptr){
            continue;
        }
        
        // if its a button reset its press down
        button->setDown(false);

        // we need to check if the button is currently being pressed down
        Vec2 curVec = InputController::getInputVector();
		if (button->containsScreen(curVec)) {
			button->setDown(true);
		}
        
        Vec2 vec = InputController::getPrevVector();
        
        // if not clicked or is not a press up we skip
        if (!button->containsScreen(vec)||!InputController::getIsPressedUp()){
            continue;
        }

		button->setDown(false);
        
        // execute the appropriate action
        switch(uiElement->getAction()->type){
            case ButtonActionType::MENU_CHANGE:
            {
                std::shared_ptr<MenuChangeButtonAction> action = std::dynamic_pointer_cast<MenuChangeButtonAction>(uiElement->getAction());
                getMenuGraph()->setActiveMenu(action->menuTarget);
                break;
            }
            case ButtonActionType::MODE_CHANGE:
            {
                std::shared_ptr<ModeChangeButtonAction> action = std::dynamic_pointer_cast<ModeChangeButtonAction>(uiElement->getAction());
                getMenuGraph()->setNextMode(action->modeTarget);
                switch(action->modeTarget){
                    case Mode::GAMEPLAY:
                    {
                        _selectedLevel = action->levelSelectDataKey;
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
            case ButtonActionType::FX_TRIGGER:
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
					case FxTriggerButtonAction::FXType::SWITCH_CHAPTER:
					{
						std::string cData = action->chapterData;
						std::shared_ptr<Event> chapterSwitchEvent = ChapterSwitchEvent::alloc();
                        
                        // gameplay controller is listening to menuController
						notify(chapterSwitchEvent.get()); // don't think anything is really listening to this?
                        
						getMenuGraph()->populateChapter(_assets, cData);
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
            break;
        }
        break;
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
                          std::shared_ptr<MenuGraph> menuGraph,
						  std::shared_ptr<GenericAssetManager> assets){
    _scene = scene;
    _menuGraph = menuGraph;
    _selectedLevel = "";
	_assets = assets;
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
