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

void MenuController::attach(std::shared_ptr<Observer> obs) {
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

bool MenuController::init(std::shared_ptr<cugl::Scene> scene){
    _scene = scene;
    _menuGraph = MenuGraph::alloc();
    return true;
}

bool MenuController::init(std::shared_ptr<Scene> scene, const std::shared_ptr<GenericAssetManager>& assets) {
    _scene = scene;
    _menuGraph = MenuGraph::alloc(assets);
    return true;
}

Mode MenuController::getMode(){
    return _menuGraph->getMode();
}

void MenuController::setMode(Mode m){
    _menuGraph->setMode(m);
}

void MenuController::activate(){
    _menuGraph->attachToScene(_scene);
}

void MenuController::deactivate(){
    _menuGraph->detachFromScene();
}
