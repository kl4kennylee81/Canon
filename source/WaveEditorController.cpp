//
//  LevelEditorController.cpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "WaveEditorController.hpp"
#include "Util.hpp"
#include "InputController.hpp"
#include "GameEngine.hpp"

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
    checkKeyboardInput();
    switch (_state) {
	case WaveEditorState::START: {
		setSceneGraph();
		updateTemplateNodes();
        updateWaveEntryNodes();
		_state = WaveEditorState::DRAG;
		break;
	}
	case WaveEditorState::NEW_TEMPLATE: {
        std::string name = "kyle" + std::to_string(_templates.size());
		auto newTemplate = TemplateWaveEntry::alloc(name, "object1", "object2", "static", {});
		_templates.push_back(newTemplate);
		updateTemplateNodes();
        createTemplateFile(newTemplate);
        //TODO: Make new file for template
		_state = WaveEditorState::DRAG;
		break;
	}
	case WaveEditorState::SELECT: {
		//allow user to select from available wave entry templates
		//switch to drag when one is clicked
		break;
	}
	case WaveEditorState::DRAG: {
        updateDragAndDrop();
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
    case WaveEditorState::COLOR_TOGGLE: {
        if(_colorChanged) {
            updateWaveEntryNodes();
            _colorChanged = false;
        }
        break;
    }
	case WaveEditorState::TEMPLATE_EDIT: {
		if (_templateEditorController->update(timestep, menuGraph)) {
			_state = WaveEditorState::SELECT;
		}
		break;
	}
	case WaveEditorState::DONE: {
		deactivateAndClear(_levelEditNode);
		return true;
	}
	}
    _wasPressed = InputController::getIsPressed();
    
    _levelEditNode->removeChildByName("label");
    std::string str = getStateAsString();
    auto label = Label::alloc(str, _world->getAssetManager()->get<Font>("kyle"));
    label->setPosition(400,20);
    _levelEditNode->addChildWithName(label, "label");
	return false;
}

void WaveEditorController::createTemplateFile(std::shared_ptr<TemplateWaveEntry> templ) {
    auto templates = JsonValue::allocObject();
    templates->appendChild(templ->name + "_template", templ->toJsonValue());
    
    auto json = JsonValue::allocObject();
    json->appendChild("templates", templates);
    
    std::ofstream newFile;
    newFile.open(TEMPLATE_PATH+templ->name+".json");
    newFile << json->toString();
    newFile.close();
}

void WaveEditorController::updateDragAndDrop(){
    if(_dragIndex == -1) return;
    auto templateNode = _levelEditNode->getChildByName("templates");
    if(_dragStart){
        for(auto it: templateNode->getChildren()){
            it->setVisible(false);
        }
        auto poly = PolygonNode::alloc(Rect::Rect(0,0,30,30));
        poly->setColor(Color4::MAGENTA);
        _dragNode->addChildWithTag(poly, 1);
        _dragStart = false;
    }
    
    bool isPressed = InputController::getIsPressed();
    Vec2 position = InputController::getInputVector();
    Vec2 scene_pos = _levelEditNode->getScene()->getCamera()->screenToWorldCoords(position);
    
    if(!isPressed && _wasPressed){
        //TODO: Either create wave entry based on dragIndex and location, or update existing wave entry's position
        _dragNode->removeAllChildren();
        if(_newEntry){
            auto templ = _templates.at(_dragIndex);
            auto entry = WaveEntry::alloc(templ->blueObjectKey, templ->aiKey, 0, 0, Element::BLUE, templ->zoneKeys);
            entry->templateIndex = _dragIndex;
            entry->position = scene_pos;
            _currentWave->addWaveEntry(entry);
        }
        else{
            auto entry = _currentWave->getEntry(_dragIndex);
            entry->position = scene_pos;
        }
        for(auto it: templateNode->getChildren()){
            it->setVisible(true);
        }
        updateWaveEntryNodes();
        _dragIndex = -1;
        return;
    }
    
    auto node = _dragNode->getChildByTag(1);
    node->setPosition(scene_pos);
}

void WaveEditorController::checkKeyboardInput() {
    auto keys = Input::get<Keyboard>()->keySet();
    for(auto key: keys){
        if(key == KeyCode::D){
            _state = WaveEditorState::DRAG;
            return;
        }
        if(key == KeyCode::E){
            _state = WaveEditorState::EDIT;
            return;
        }
        if(key == KeyCode::T){
            _state = WaveEditorState::COLOR_TOGGLE;
            return;
        }
    }

}

void WaveEditorController::setWave(std::shared_ptr<WaveData> wave) {
	_currentWave = wave;
	_state = WaveEditorState::START;
}

void WaveEditorController::templateButtonListenerFunction(const std::string& name, bool down, int index){
    auto templateNode = _levelEditNode->getChildByName("templates");
    auto buttonNode = std::static_pointer_cast<Button>(templateNode->getChildByTag(index));
    if (buttonNode->isDown()) {
        switch(_state) {
        case WaveEditorState::DRAG: {
            _dragIndex = index;
            _newEntry = true;
            _dragStart = true;
            break;
        }
        case WaveEditorState::EDIT: {
            break;
        }
        default:{
            break;
        }
        }
        
    }
}


void WaveEditorController::waveEntryButtonListenerFunction(const std::string& name, bool down, int index) {
    auto entryNode = _levelEditNode->getChildByName("entries");
    auto buttonNode = std::static_pointer_cast<Button>(entryNode->getChildByTag(index));
    if(buttonNode->isDown()){
        switch(_state) {
            case WaveEditorState::DRAG: {
                _dragIndex = index;
                _newEntry = false;
                _dragStart = true;
                break;
            }
            case WaveEditorState::EDIT: {
                break;
            }
            case WaveEditorState::COLOR_TOGGLE: {
                auto waveEntry = _currentWave->getEntry(index);
                auto templateEntry = _templates.at(waveEntry->templateIndex);
                waveEntry->switchElement(templateEntry->blueObjectKey, templateEntry->goldObjectKey);
                _colorChanged = true;
                break;
            }
            default:{
                break;
            }
        }
    }
}

void WaveEditorController::updateWaveEntryNodes(){
    auto entryNode = _levelEditNode->getChildByName("entries");
    deactivateAndClear(entryNode);
    auto waveEntries = _currentWave->getWaveEntries();
    for(int i = 0; i < waveEntries.size(); i++){
        auto entry = waveEntries.at(i);
        Vec2 pos = entry->position;
        Color4 color = entry->element == Element::BLUE ? Color4::BLUE : Color4::YELLOW;
        auto waveEntryButton = Util::makeBoxButton(pos.x, pos.y, 30, 30, color, Color4::PAPYRUS);
        waveEntryButton->setListener(
            [=](const std::string& name, bool down) {
                waveEntryButtonListenerFunction(name, down, i);
            }
        );
        waveEntryButton->activate(getUid());
        entryNode->addChildWithTag(waveEntryButton, i);
    }
}


void WaveEditorController::updateTemplateNodes() {
	auto templateNode = _levelEditNode->getChildByName("templates");
	deactivateAndClear(templateNode);
	for (int i = 0; i < _templates.size(); i++) {
		auto templateButton = Util::makeBoxButton(30, 500 - (i * 40), 30, 30, Color4::BLACK, Color4::PAPYRUS);
		templateButton->setListener(
			[=](const std::string& name, bool down) {
                templateButtonListenerFunction(name, down, i);
            }
		);
		templateButton->activate(getUid());
		templateNode->addChildWithTag(templateButton, i);
	}
}

std::string WaveEditorController::getStateAsString(){
    switch(_state){
        case WaveEditorState::DRAG: {
            return "DRAG";
        }
        case WaveEditorState::COLOR_TOGGLE: {
            return "COLOR TOGGLE";
        }
        case WaveEditorState::EDIT: {
            return "EDIT";
        }
    }
    return "";
}

void WaveEditorController::setSceneGraph() {
	deactivateAndClear(_levelEditNode);

	auto backButton = Util::makeBoxButton(30, 30, 30, 30, Color4::RED, Color4::PAPYRUS);
	backButton->setListener(
		[=](const std::string& name, bool down) {
		if (down) {
			_state = WaveEditorState::DONE;
		}
	}
	);

	auto addButton = Util::makeBoxButton(70, 30, 30, 30, Color4::GREEN, Color4::PAPYRUS);
	addButton->setListener(
		[=](const std::string& name, bool down) {
		if (down) {
			_state = WaveEditorState::NEW_TEMPLATE;
		}
	}
	);

	backButton->activate(getUid());
	addButton->activate(getUid());
	_levelEditNode->addChildWithName(backButton, "back");
	_levelEditNode->addChildWithName(addButton, "add");
	_levelEditNode->addChildWithName(Node::alloc(), "templates");
    _levelEditNode->addChildWithName(Node::alloc(), "entries");
    
    _dragNode = Node::alloc();
    _levelEditNode->addChildWithName(_dragNode, "drag");
    _dragIndex = -1;
}

bool WaveEditorController::init(std::shared_ptr<Node> node, std::shared_ptr<World> world) {
	_state = WaveEditorState::START;
	_templateEditorController = TemplateEditorController::alloc(node, world);
	_levelEditNode = node;
	_world = world;
    _colorChanged = false;
    _templates.push_back(world->getAssetManager()->get<TemplateWaveEntry>("kyle0"));
    _templates.push_back(world->getAssetManager()->get<TemplateWaveEntry>("kyle1"));
	return true;
}
