//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TemplateEditorController.hpp"

using namespace cugl;

void TemplateEditorController::attach(Observer* obs) {
	BaseController::attach(obs);
}

void TemplateEditorController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void TemplateEditorController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void TemplateEditorController::eventUpdate(Event* e) {

}

/**
* Returns true when we are editing the template wave entry (the user has pressed the back/done button).
* Returns false otherwise.
*/
bool TemplateEditorController::update(float timestep, std::shared_ptr<MenuGraph> menuGraph) {
	return true;
}

void TemplateEditorController::setTemplate(std::shared_ptr<WaveEntry> entry) {

}

bool TemplateEditorController::init(std::shared_ptr<Node> node, std::shared_ptr<GenericAssetManager> assets) {
	return true;
}
