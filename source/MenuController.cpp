//
//  MenuController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "MenuController.hpp"

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
    
    
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void MenuController::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    this->_scene->render(_batch);
}

bool MenuController::init(std::shared_ptr<cugl::Scene> scene,
                          std::shared_ptr<MenuGraph> menuGraph){
    _scene = scene;
    _menuGraph = menuGraph;
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
