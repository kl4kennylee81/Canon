//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "LevelEditorController.hpp"
#include "LevelEditorEvent.hpp"
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
    case LevelEditorState::START: {
        // need to activate the button during the update loop after it is garunteed that
        // all previous button have been deactivated which happens after initialization.
        setSceneGraph();
        loadLevel("editorLevel.json");
        updateWaveNodes();
        _state = LevelEditorState::MAIN;
        break;
    }
	case LevelEditorState::MAIN: {
		//Look for clicks so we can enter wave editor
		break;
	}
	case LevelEditorState::SWITCH_TO_WAVE: {
		deactivateAndClear(_levelEditNode);
		_state = LevelEditorState::WAVE;
		break;
	}
	case LevelEditorState::ADD_NEW_WAVE: {
		addNewWave();
		updateWaveNodes();
        saveLevel();
		_state = LevelEditorState::MAIN;
		break;
	}
	case LevelEditorState::WAVE: {
		if (_waveEditorController->update(timestep, menuGraph)) {
			_state = LevelEditorState::MAIN;
			setSceneGraph();
			updateWaveNodes();
            saveLevel();
		}
		break;
	}
	}
}

void LevelEditorController::loadLevel(std::string file){
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    std::string canonDir = Util::join(vec,vec.size()-2,'/');
    auto reader = JsonReader::alloc("/"+canonDir+"/assets/json/"+file);
    if(reader == nullptr) return;
    auto json = reader->readJson();
    
    auto templates = json->get("templates");
    _waveEditorController->setTemplates(templates->asStringArray());
    
    _levelData = LevelData::alloc();
    _levelData->preload(json->get("level"));
    
    std::shared_ptr<JsonValue> waves = json->get("waves");
    for(int i = 0; i < _levelData->getNumberWaves(); i++){
        auto waveData = WaveData::alloc();
        waveData->preload(waves->get(i));
        _world->_waveData[_levelData->getWaveKey(i)] = waveData;
    }
    
}

void LevelEditorController::saveLevel() {
    auto json = JsonValue::allocObject();
    
    auto templates = JsonValue::allocArray();
    for(auto it: _waveEditorController->getTemplates()){
        templates->appendValue(it->getName());
    }
    json->appendChild("templates", templates);
    
    json->appendChild("level", _levelData->toJsonValue());
    
    auto waves = JsonValue::allocObject();
    for(int i = 0; i < _levelData->getNumberWaves(); i++){
        std::string key = _levelData->getWaveKey(i);
        waves->appendChild(key, _world->getWaveData(key)->toJsonValue());
    }
    json->appendChild("waves", waves);
    
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    std::string assetDir = Util::join(vec,vec.size()-2,'/');
    std::ofstream newFile;
    newFile.open("/"+assetDir+"/assets/json/editorLevel.json");
    newFile << json->toString();
    newFile.close();
}

/** to add it to the scene graph node */
void LevelEditorController::activate(std::shared_ptr<Scene> scene){
    scene->addChild(this->_levelEditNode);
}

/** to remove it from hte scene graph node */
void LevelEditorController::deactivate(std::shared_ptr<Scene> scene){
    scene->removeChild(this->_levelEditNode);
}

void LevelEditorController::setSceneGraph() {
	deactivateAndClear(_levelEditNode);
	auto backButton = Util::makeBoxButton(30, 30, 30, 30, Color4::RED, Color4::PAPYRUS);

    backButton->setListener(
        [=](const std::string& name, bool down) {
            if (down) {
                _waveEditorController->updateWorld(_world);
                std::shared_ptr<Event> slEvent = SimulateLevelEvent::alloc();
                this->notify(slEvent.get());
            }
        }
    );
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
    std::string waveName = "wave"+ std::to_string(_levelData->getNumberWaves());
	std::shared_ptr<LevelEntry> entry = LevelEntry::alloc(waveName, 180);
    _world->_waveData[waveName] = WaveData::alloc();
	_levelData->addLevelEntry(entry);
}


void LevelEditorController::updateWaveNodes() {
	auto waveNode = _levelEditNode->getChildByName("waves");
	deactivateAndClear(waveNode);
	for (int i = 0; i < _levelData->getNumberWaves(); i++) {
		auto waveButton = Util::makeBoxButton(200 + 40 * i, 200, 30, 30, Color4::BLACK, Color4::PAPYRUS);
		waveButton->setListener(
			[=](const std::string& name, bool down) {
			auto buttonNode = std::static_pointer_cast<Button>(waveNode->getChildByTag(i));
			if (buttonNode->isDown()) {
				auto waveData = _world->getWaveData(_levelData->getWaveKey(i));
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
	_state = LevelEditorState::START;
	_levelEditNode = Node::alloc();
    this->activate(scene);
    
    // TODO need to refresh and hold a list of levelData that are already made
    
	_levelData = LevelData::alloc();
    _world = World::alloc(assets,_levelData);
    _world->_isSandbox = true;
    _waveEditorController = WaveEditorController::alloc(_levelEditNode, _world);
    return true;
}

void LevelEditorController::dispose(){
    _world = nullptr;
    _levelEditNode = nullptr;
    _waveEditorController = nullptr;
    _levelData = nullptr;
}
