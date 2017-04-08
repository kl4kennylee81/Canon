//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelEditorController.hpp"
#include "Util.hpp"

using namespace cugl;

void LevelEditorController::attach(Observer* obs){
    BaseController::attach(obs);
}

void LevelEditorController::detach(Observer* obs){
    BaseController::detach(obs);
}

void LevelEditorController::notify(Event* e){
    BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void LevelEditorController::eventUpdate(Event* e){

}

void LevelEditorController::update(float timestep,std::shared_ptr<MenuGraph> menuGraph){
	switch (_state) {
	case LevelEditorState::MAIN: {
		//Look for clicks so we can enter wave editor
		break;
	}
	case LevelEditorState::SWITCH_TO_WAVE: {
		clearNodes();
		_state = LevelEditorState::WAVE;
		break;
	}
	case LevelEditorState::ADD_NEW_WAVE: {
		addNewWave();
		updateWaveNodes();
		_state = LevelEditorState::MAIN;
		break;
	}
	case LevelEditorState::WAVE: {
		if (_waveEditorController->update(timestep, menuGraph)) {
			_state = LevelEditorState::MAIN;
			setSceneGraph();
			updateWaveNodes();
		}
		break;
	}
	}
}

void LevelEditorController::setSceneGraph() {
	clearNodes();

	auto backButton = Util::makeBoxButton(30, 30, 30, 30, Color4::RED, Color4::PAPYRUS);
	backButton->activate(getUid());

	auto addButton = Util::makeBoxButton(70, 30, 30, 30, Color4::GREEN, Color4::PAPYRUS);
	addButton->setListener(
		[=](const std::string& name, bool down) {
		if (down) {
			_state = LevelEditorState::ADD_NEW_WAVE;
		}
		}
	);

	addButton->activate(getUid());

	_levelEditNode->addChildWithName(addButton, "add");
	_levelEditNode->addChildWithName(backButton, "back");

	auto waves = Node::alloc();
	_levelEditNode->addChildWithName(waves, "waves");
}

void LevelEditorController::addNewWave() {
	std::shared_ptr<LevelEntry> entry = LevelEntry::alloc("wave"+_levelData->getNumberWaves(), 2);
	_levelData->addLevelEntry(entry);
}

void LevelEditorController::clearWaveNodes() {
	auto waveNode = _levelEditNode->getChildByName("waves");
	if (waveNode == nullptr) return;

	for (auto node : waveNode->getChildren()) {
		auto buttonNode = std::static_pointer_cast<Button>(node);
		buttonNode->deactivate();
	}

	waveNode->removeAllChildren();
}

//Necessary to deactivate all buttons before clearing
void LevelEditorController::clearNodes() {
	deactivateButton(_levelEditNode, "add");
	deactivateButton(_levelEditNode, "back");
	clearWaveNodes();
	_levelEditNode->removeAllChildren();
}

void LevelEditorController::updateWaveNodes() {
	clearWaveNodes();
	auto waveNode = _levelEditNode->getChildByName("waves");
	for (int i = 0; i < _levelData->getNumberWaves(); i++) {
		auto waveButton = Util::makeBoxButton(200 + 40 * i, 200, 30, 30, Color4::BLACK, Color4::PAPYRUS);
		waveButton->setListener(
			[=](const std::string& name, bool down) {
			auto buttonNode = std::static_pointer_cast<Button>(waveNode->getChildByTag(i));
			if (buttonNode->isDown()) {
				auto waveData = _assets->get<WaveData>(_levelData->getWaveKey(i));
				_waveEditorController->setWave(waveData);
				_state = LevelEditorState::SWITCH_TO_WAVE;
			}
		}
		);
		waveButton->activate(getUid());
		waveNode->addChildWithTag(waveButton, i);
	}
}

bool LevelEditorController::init(std::shared_ptr<Scene> scene, std::shared_ptr<GenericAssetManager> assets){
	_state = LevelEditorState::MAIN;
	_levelEditNode = Node::alloc();
	_levelData = LevelData::alloc();
	_waveEditorController = WaveEditorController::alloc(_levelEditNode, assets);
	scene->addChild(_levelEditNode);
	setSceneGraph();
	_assets = assets;
    return true;
}
