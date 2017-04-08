//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "WaveEditorController.hpp"
#include "Util.hpp"

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
	case WaveEditorState::START: {
		clearNodes();
		auto backButton = Util::makeBoxButton(30, 30, 30, 30, Color4::RED, Color4::PAPYRUS);
		backButton->activate(1);
		backButton->setListener(
			[=](const std::string& name, bool down) {
			if (down) {
				_state = WaveEditorState::DONE;
			}
		}
		);

		_levelEditNode->addChildWithName(backButton, "back");
		_state = WaveEditorState::SELECT;
		break;
	}
	case WaveEditorState::SELECT: {
		//allow user to select from available wave entry templates
		//switch to drag when one is clicked
		break;
	}
	case WaveEditorState::DRAG: {
		//click on item to start dragging, click again to drop
		break;
	}
	case WaveEditorState::EDIT: {
		//click on item to start editing, switch to TEMPLATE_EDIT
		break;
	}
	case WaveEditorState::REMOVE: {
		//click on item to remove
		break;
	}
	case WaveEditorState::TEMPLATE_EDIT: {
		if (_templateEditorController->update(timestep, menuGraph)) {
			_state = WaveEditorState::SELECT;
		}
		break;
	}
	case WaveEditorState::DONE: {
		clearNodes();
		return true;
	}
	}
	return false;
}

void WaveEditorController::clearNodes() {
	auto backNode = _levelEditNode->getChildByName("back");
	if (backNode != nullptr) {
		auto backButton = std::static_pointer_cast<Button>(backNode);
		backButton->deactivate();
	}
	_levelEditNode->removeAllChildren();
}

void WaveEditorController::setWave(std::shared_ptr<WaveData> wave) {
	_currentWave = wave;
	_state = WaveEditorState::START;
}

bool WaveEditorController::init(std::shared_ptr<Node> node, std::shared_ptr<GenericAssetManager> assets) {
	_state = WaveEditorState::START;
	_templateEditorController = TemplateEditorController::alloc(node, assets);
	_levelEditNode = node;
	_assets = assets;
	return true;
}
