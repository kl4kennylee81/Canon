//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "WaveEditorController.hpp"

using namespace cugl;

void WaveEditorController::attach(Observer* obs) {
	BaseController::attach(obs);
}

void WaveEditorController::detach(Observer* obs) {
	BaseController::detach(obs);
}

void WaveEditorController::notify(Event* e) {
	BaseController::notify(e);
}

/**
* Update the observer state based on an event from the subject
*/
void WaveEditorController::eventUpdate(Event* e) {

}

/**
* Returns true when we are editing the wave (the user has pressed the back/done button).
* Returns false otherwise.
*/
bool WaveEditorController::update(float timestep, std::shared_ptr<MenuGraph> menuGraph) {
	switch (_state) {
	case WaveEditorState::SELECT: {
		//allow user to select from available wave entry templates
		//switch to drag when one is clicked
	}
	case WaveEditorState::DRAG: {
		//click on item to start dragging, click again to drop
	}
	case WaveEditorState::EDIT: {
		//click on item to start editing, switch to TEMPLATE_EDIT
	}
	case WaveEditorState::REMOVE: {
		//click on item to remove
	}
	case WaveEditorState::TEMPLATE_EDIT: {
		if (_templateEditorController->update(timestep, menuGraph)) {
			_state = WaveEditorState::SELECT;
		}
	}
	}
	return true;
}

void WaveEditorController::setWave(std::shared_ptr<WaveData> wave) {

}

bool WaveEditorController::init(std::shared_ptr<Node> node, std::shared_ptr<GenericAssetManager> assets) {
	return true;
}
