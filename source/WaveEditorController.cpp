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
#include "ActiveAnimation.hpp"
#include "AnimationData.hpp"
#include "GameState.hpp"
#include "AIData.hpp"
#include "ZoneData.hpp"
#include "GenericAssetManager.hpp"

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
        auto newTemplate = TemplateWaveEntry::alloc(name, "object1", {"static"}, {},90.0, "");

		_templates.push_back(newTemplate);
        _world->addTemplate(name, newTemplate);
		updateTemplateNodes();
        createTemplateFile(newTemplate);
        //TODO: Make new file for template
		_state = WaveEditorState::DRAG;
		break;
	}
    case WaveEditorState::REFRESH: {
        refreshTemplates();
        _state = _prevState;
        break;
    }
	case WaveEditorState::DRAG: {
        updateDragAndDrop();
        if(_showTemplates){
            updateTemplateNodes();
        }
        else {
            deactivateAndClear(_levelEditNode->getChildByName("templates"));
        }
		break;
	}
	case WaveEditorState::REMOVE: {
        deactivateAndClear(_levelEditNode->getChildByName("templates"));
        if(_entryRemoved){
            _currentWave->removeEntry(_removeIndex);
            updateWaveEntryNodes();
            _entryRemoved = false;
        }
        break;
	}
    case WaveEditorState::AI_TOGGLE: {
        deactivateAndClear(_levelEditNode->getChildByName("templates"));
        if(_aiChanged) {
            updateWaveEntryNodes();
            _aiChanged = false;
        }
    }
    case WaveEditorState::ZONE_TOGGLE: {
        deactivateAndClear(_levelEditNode->getChildByName("templates"));
        if(_zoneChanged) {
            updateWaveEntryNodes();
            _zoneChanged = false;
        }
    }
    case WaveEditorState::COLOR_TOGGLE: {
        deactivateAndClear(_levelEditNode->getChildByName("templates"));
        if(_colorChanged) {
            updateWaveEntryNodes();
            _colorChanged = false;
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
    auto label = Label::alloc(str, _world->getAssetManager()->get<Font>("Charlemagne_40"));
    label->setPosition(400,20);
    label->setScale(0.6);
    _levelEditNode->addChildWithName(label, "label");
	return false;
}



/* -------------------------------------------------------------- */
/* -------------------- TEMPLATE FILE STUFF --------------------- */
/* -------------------------------------------------------------- */

void WaveEditorController::refreshTemplates(){
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    std::string canonDir = Util::join(vec,vec.size()-2,'/');
    
    std::unordered_set<std::string> aiKeys;
    std::unordered_set<std::string> zoneKeys;
    std::unordered_set<std::string> templateKeys;
    auto tempAssets = GenericAssetManager::alloc();
    GameEngine::attachLoaders(tempAssets);

    //get all keys
    for(auto it: _templates){
        auto reader = JsonReader::alloc("/"+canonDir+"/assets/json/templates/"+it->getName()+".json");
        auto json = reader->readJson();
        if(json->has("ai")){
            auto ais = json->get("ai");
            for(int i = 0; i < ais->size(); i++){
                aiKeys.insert(ais->get(i)->key());
            }
        }
        if(json->has("zones")){
            auto zones = json->get("zones");
            for(int i = 0; i < zones->size(); i++){
                zoneKeys.insert(zones->get(i)->key());
            }
        }
        if(json->has("templates")){
            auto templates = json->get("templates");
            for(int i = 0; i < templates->size(); i++){
                templateKeys.insert(templates->get(i)->key());
            }
        }
        tempAssets->loadDirectory(json);
    }
    
    //transfer data into world
    for(auto it: aiKeys){
        _world->_aiData[it] = tempAssets->get<AIData>(it);
    }
    for(auto it: zoneKeys){
        _world->_zoneData[it] = tempAssets->get<ZoneData>(it);
    }
    for(auto it: templateKeys){
        _world->_templateData[it] = tempAssets->get<TemplateWaveEntry>(it);
    }

    //update _templates to reflect changes
    std::vector<std::shared_ptr<TemplateWaveEntry>> newTemplates;
    for(auto it: _templates){
        newTemplates.push_back(_world->getTemplate(it->getName()));
    }
    _templates = newTemplates;
    
    tempAssets->unloadAll();
}

void WaveEditorController::createTemplateFile(std::shared_ptr<TemplateWaveEntry> templ) {
    auto templates = JsonValue::allocObject();
    templates->appendChild(templ->name, templ->toJsonValue());
    
    auto json = JsonValue::allocObject();
    json->appendChild("templates", templates);
    
    std::vector<std::string> vec = Util::split(__FILE__, '/');
    std::string assetDir = Util::join(vec,vec.size()-2,'/');
    std::ofstream newFile;
    newFile.open("/"+assetDir+"/assets/json/templates/"+templ->name+".json");
    newFile << json->toString();
    newFile.close();
}




/* -------------------------------------------------------------- */
/* ------------------ DRAG AND DROP FUNCTION -------------------- */
/* -------------------------------------------------------------- */

void WaveEditorController::updateDragAndDrop(){
    if(_dragIndex == -1) return;
    auto templateNode = _levelEditNode->getChildByName("templates");
    if(_dragStart){
        for(auto it: templateNode->getChildren()){
            it->setVisible(false);
        }
        auto poly = PolygonNode::alloc(Rect::Rect(0,0,20,20));
        poly->setColor(Color4::BLACK);
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
            auto entry = WaveEntry::alloc(0, 0, ElementType::BLUE, templ->name, templ->getAIKey(0));

            
            Vec2 physicsCoord;
            Util::sceneToPhysicsCoords(scene_pos,physicsCoord);
            entry->setPosition(physicsCoord);
            _currentWave->addWaveEntry(entry);
        }
        else{
            auto entry = _currentWave->getEntry(_dragIndex);
            Vec2 physicsCoord;
            Util::sceneToPhysicsCoords(scene_pos,physicsCoord);
            entry->setPosition(physicsCoord);
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




/* -------------------------------------------------------------- */
/* ---------------------- TOGGLING STUFF ------------------------ */
/* -------------------------------------------------------------- */

std::string WaveEditorController::getStateAsString(){
    switch(_state){
        case WaveEditorState::DRAG: {
            return "DRAG";
        }
        case WaveEditorState::COLOR_TOGGLE: {
            return "COLOR TOGGLE";
        }
        case WaveEditorState::REMOVE: {
            return "REMOVE";
        }
        case WaveEditorState::AI_TOGGLE: {
            return "AI TOGGLE";
        }
        case WaveEditorState::ZONE_TOGGLE: {
            return "ZONE TOGGLE";
        }
    }
    return "";
}

void WaveEditorController::checkKeyboardInput() {
    auto keys = Input::get<Keyboard>()->keySet();
    for(auto key: keys){
        if(key == KeyCode::D){
            _state = WaveEditorState::DRAG;
            return;
        }
        if(key == KeyCode::BACKSPACE){
            _state = WaveEditorState::REMOVE;
            return;
        }
        if(key == KeyCode::C){
            _state = WaveEditorState::COLOR_TOGGLE;
            return;
        }
        if(key == KeyCode::A){
            _state = WaveEditorState::AI_TOGGLE;
            return;
        }
        if(key == KeyCode::Z){
            _state = WaveEditorState::ZONE_TOGGLE;
            return;
        }
        if(key == KeyCode::NUM_1){
            if(_state == WaveEditorState::DRAG){
                _showTemplates = true;
            }
        }
        if(key == KeyCode::NUM_2){
            if(_state == WaveEditorState::DRAG){
                _showTemplates = false;
            }
        }
    }

}

/* -------------------------------------------------------------- */
/* ---------------- BUTTON LISTENER FUNCTIONS ------------------- */
/* -------------------------------------------------------------- */

void WaveEditorController::waveEntryButtonListenerFunction(const std::string& name, bool down, int index) {
    auto entryNode = _levelEditNode->getChildByName("entries");
    auto buttonNode = std::static_pointer_cast<Button>(entryNode->getChildByTag(index));
    if(down){
        switch(_state) {
            case WaveEditorState::DRAG: {
                _dragIndex = index;
                _newEntry = false;
                _dragStart = true;
                break;
            }
            case WaveEditorState::REMOVE: {
                _entryRemoved = true;
                _removeIndex = index;
                break;
            }
            case WaveEditorState::COLOR_TOGGLE: {
                auto waveEntry = _currentWave->getEntry(index);
                auto templateEntry = this->getTemplateWaveEntry(waveEntry->getTemplateKey());
                waveEntry->switchElement();
                _colorChanged = true;
                break;
            }
            case WaveEditorState::AI_TOGGLE: {
                auto waveEntry = _currentWave->getEntry(index);
                auto templateEntry = this->getTemplateWaveEntry(waveEntry->getTemplateKey());
                waveEntry->setAIKey(templateEntry->getNextAIKey(waveEntry->getAIKey()));
                _aiChanged = true;
                break;
            }
            case WaveEditorState::ZONE_TOGGLE: {
                auto waveEntry = _currentWave->getEntry(index);
                auto templateEntry = this->getTemplateWaveEntry(waveEntry->getTemplateKey());
                waveEntry->setZoneKeys(templateEntry->getNextZoneKeys(waveEntry->getZoneKeys()));
                _zoneChanged = true;
                break;
            }
            default:{
                break;
            }
        }
    }
}

void WaveEditorController::templateButtonListenerFunction(const std::string& name, bool down, int index){
    auto templateNode = _levelEditNode->getChildByName("templates");
    auto buttonNode = std::static_pointer_cast<Button>(templateNode->getChildByTag(index));
    if (down) {
        switch(_state) {
            case WaveEditorState::DRAG: {
                _dragIndex = index;
                _newEntry = true;
                _dragStart = true;
                break;
            }
        }
    }
}





/* -------------------------------------------------------------- */
/* -------------- SCENE GRAPH UTILITY FUNCTIONS ----------------- */
/* -------------------------------------------------------------- */

std::shared_ptr<Button> WaveEditorController::getButtonFromTemplate(float x, float y,
                                                                    std::shared_ptr<TemplateWaveEntry> templ, ElementType color)
{
    auto objectData = _world->getObjectData(templ->getObjectKey());
    auto animData = _world->getAnimationData(objectData->getAnimationKey(color));
    
    auto activeAnim = ActiveAnimation::alloc(objectData->getAnimationScale());
    activeAnim->setAnimationData(animData);
    auto button = Button::alloc(activeAnim->getAnimationNode());
    button->setPosition(x, y);
    button->setScale(getAnimationScale(templ->getObjectKey(),animData->nonUniformScale));
    return button;
}

Vec2 WaveEditorController::getAnimationScale(std::string objectKey,bool isNonUniform){
    std::shared_ptr<ObjectData> obj = _world->getObjectData(objectKey);
    std::shared_ptr<ShapeData> shape = _world->getShapeData(obj->getShapeKey());
    Poly2 poly(shape->vertices);
    SimpleTriangulator triangulator;
    triangulator.set(poly);
    triangulator.calculate();
    poly.setIndices(triangulator.getTriangulation());
    std::shared_ptr<PolygonObstacle> obst = PolygonObstacle::alloc(poly);
    
    // TODO replace this hacky multiply by physics scale
    Size polySize = Size::Size(obst->getSize().width*GAME_PHYSICS_SCALE,obst->getSize().height*GAME_PHYSICS_SCALE);
    
    auto activeAnim = ActiveAnimation::alloc(obj->getAnimationScale());
    auto animData = _world->getAnimationData(obj->getAnimationKey(ElementType::GOLD));
    activeAnim->setAnimationData(animData);
    
    auto anim = activeAnim->getAnimationNode();
    
    Size animationSize = anim->getContentSize();
    
    // maximum of boxSize.width/animation.width and boxsize.height/animation.height
    // so that the animationNode is always encapsulating the full physics box with padding.
    // this is for if we wanted to increase the size of the character, you'd only have to increase the physics box size
    if (isNonUniform){
        float scaleX = (polySize.width)/animationSize.width;
        float scaleY = (polySize.height)/animationSize.height;
        Vec2 animationScale = Vec2::Vec2(scaleX,scaleY) * activeAnim->getAnimationScale();
        return animationScale;
    } else {
        float scaleX = (polySize.width)/animationSize.width;
        float scaleY = (polySize.height)/animationSize.height;
        float animationScale = std::max(scaleX,scaleY) * activeAnim->getAnimationScale();
        return Vec2::Vec2(animationScale,animationScale);
    }

}

std::shared_ptr<Node> WaveEditorController::createZoneNode(float x, // this is in physicsCoordinates of the owner of the zone
                                                           float y, // this is in physicsCoordinates of the owner of the zone
                                                           std::string zoneKey, ElementType parentColor){
    auto zoneD = _world->getZoneData(zoneKey);
    switch(zoneD->type){
        case ZoneType::STATIC:
        {
            auto staticZoneD =  std::dynamic_pointer_cast<StaticZoneData>(zoneD);
            auto zoneObjData = _world->getObjectData(staticZoneD->objectKey);
            auto zoneAnim = ActiveAnimation::alloc(zoneObjData->getAnimationScale());
            ElementType zoneElement = Element::elementDataTypeToElement(staticZoneD->elementType,parentColor);
            auto staticAnimD = _world->getAnimationData(zoneObjData->getAnimationKey(zoneElement));
            zoneAnim->setAnimationData(staticAnimD);
            std::shared_ptr<Node> zoneNode = zoneAnim->getAnimationNode();
            Vec2 zonePos = staticZoneD->getPosition(Vec2::Vec2(x,y));
            Util::physicsToSceneCoords(zonePos, zonePos);
            zoneNode->setPosition(zonePos);
            zoneNode->setScale(getAnimationScale(staticZoneD->objectKey,staticAnimD->nonUniformScale));
            
            // set the color
            if (zoneElement == ElementType::BLUE) {
                zoneNode->setColor(Color4f(Color4::BLUE)*Color4f(1,1,1,0.5));
            } else {
                zoneNode->setColor(Color4f(Color4::YELLOW)*Color4f(1,1,1,0.5));
            }
            return zoneNode;
        }
        case ZoneType::ROTATE:
        {
            std::shared_ptr<Node> rotateNode = Node::alloc();
            auto rotateZoneD =  std::dynamic_pointer_cast<RotateZoneData>(zoneD);
            for (std::shared_ptr<ZoneEntry> zEntry : rotateZoneD->zones){
                auto zoneObjData = _world->getObjectData(zEntry->objectKey);
                ElementType zoneElement = Element::elementDataTypeToElement(zEntry->elementType,parentColor);
                auto zEntryAnimD = _world->getAnimationData(zoneObjData->getAnimationKey(zoneElement));
                auto zoneAnim = ActiveAnimation::alloc(zoneObjData->getAnimationScale());
                zoneAnim->setAnimationData(zEntryAnimD);
                std::shared_ptr<Node> zoneNode = zoneAnim->getAnimationNode();
                Vec2 zonePos = zEntry->getPosition(Vec2::Vec2(x,y),rotateZoneD->radius);
                Util::physicsToSceneCoords(zonePos, zonePos);
                zoneNode->setPosition(zonePos);
                zoneNode->setAngle(zEntry->getAngle());
                zoneNode->setScale(getAnimationScale(zEntry->objectKey,zEntryAnimD->nonUniformScale));
                
                // set the color
                if (zoneElement == ElementType::BLUE) {
                    zoneNode->setColor(Color4f(Color4::BLUE)*Color4f(1,1,1,0.5));
                } else {
                    zoneNode->setColor(Color4f(Color4::YELLOW)*Color4f(1,1,1,0.5));
                }
                
                rotateNode->addChild(zoneNode,0);
            }
            
            return rotateNode;
        }
        case ZoneType::PULSE:
        {
            std::shared_ptr<Node> pulseNode = Node::alloc();
            
            auto pulseZoneD =  std::dynamic_pointer_cast<PulseZoneData>(zoneD);
            auto zoneObjData = _world->getObjectData(pulseZoneD->objectKey);
            auto zoneAnim = ActiveAnimation::alloc(zoneObjData->getAnimationScale());
            ElementType zoneElement = Element::elementDataTypeToElement(pulseZoneD->elementType,parentColor);
            auto pulseAnimD = _world->getAnimationData(zoneObjData->getAnimationKey(zoneElement));
            zoneAnim->setAnimationData(pulseAnimD);
            std::shared_ptr<Node> zoneNode = zoneAnim->getAnimationNode();
            Vec2 zonePos = pulseZoneD->getPosition(Vec2::Vec2(x,y));
            Util::physicsToSceneCoords(zonePos, zonePos);
            zoneNode->setPosition(zonePos);
            Vec2 animScale = getAnimationScale(pulseZoneD->objectKey,pulseAnimD->nonUniformScale);
            zoneNode->setScale(animScale*pulseZoneD->minSize);
            
            // set the color
            if (zoneElement == ElementType::BLUE) {
                zoneNode->setColor(Color4f(Color4::BLUE)*Color4f(1,1,1,0.5));
            } else {
                zoneNode->setColor(Color4f(Color4::YELLOW)*Color4f(1,1,1,0.5));
            }
            pulseNode->addChild(zoneNode,0);
            
            pulseAnimD = _world->getAnimationData(zoneObjData->getAnimationKey(zoneElement));
            zoneAnim->setAnimationData(pulseAnimD);
            zoneNode = zoneAnim->getAnimationNode();
            zoneNode->setPosition(zonePos);
            zoneNode->setScale(getAnimationScale(pulseZoneD->objectKey,pulseAnimD->nonUniformScale)*pulseZoneD->maxSize);
            
            // set the color
            if (zoneElement == ElementType::BLUE) {
                zoneNode->setColor(Color4f(Color4::BLUE)*Color4f(1,1,1,0.5));
            } else {
                zoneNode->setColor(Color4f(Color4::YELLOW)*Color4f(1,1,1,0.5));
            }
            pulseNode->addChild(zoneNode,0);
            return pulseNode;
        }
    }
}




/* -------------------------------------------------------------- */
/* -------------- SCENE GRAPH DISPLAY FUNCTIONS ----------------- */
/* -------------------------------------------------------------- */

void WaveEditorController::updateTemplateNodes() {
    auto templateNode = _levelEditNode->getChildByName("templates");
    deactivateAndClear(templateNode);
    for (int i = 0; i < _templates.size(); i++) {
        auto button = getButtonFromTemplate(30, 500 - (i * 75), _templates.at(i), ElementType::BLUE);
        auto label = Label::alloc(_templates.at(i)->getName(), _world->getAssetManager()->get<Font>("Charlemagne_40"));
        label->setScale(0.35);
        label->setPosition(90, 500 - (i * 75));
        button->setListener(
        [=](const std::string& name, bool down) {
            templateButtonListenerFunction(name, down, i);
        }
        );
        button->activate(getUid());
        templateNode->addChildWithTag(button, i);
        templateNode->setZOrder(5000);
        templateNode->addChild(label);
    }
}


void WaveEditorController::updateWaveEntryNodes(){
    auto entryNode = _levelEditNode->getChildByName("entries");
    deactivateAndClear(entryNode);
    auto waveEntries = _currentWave->getWaveEntries();
    for(int i = 0; i < waveEntries.size(); i++){
        auto entry = waveEntries.at(i);
        Vec2 pos;
        Util::physicsToSceneCoords(entry->getPosition(),pos);
        
        auto templateData = _world->getTemplate(entry->getTemplateKey());
        auto button = getButtonFromTemplate(pos.x, pos.y, templateData, entry->getElement());
        
        // create the zones
        for (std::string zoneKey : _world->getZoneKeys(entry)){
            std::shared_ptr<Node> zoneNode = createZoneNode(entry->getPosition().x,entry->getPosition().y,zoneKey,entry->getElement());
            entryNode->addChild(zoneNode,0);
        }

        auto label = Label::alloc(entry->getAIKey(), _world->getAssetManager()->get<Font>("Charlemagne_40"));
        label->setScale(0.25);
        label->setPosition(pos.x, pos.y);
        
        button->setListener(
        [=](const std::string& name, bool down) {
            waveEntryButtonListenerFunction(name, down, i);
        }
        );
        button->activate(getUid());
        entryNode->addChildWithTag(button, i,1);
        entryNode->addChild(label,2);
    }
}

void WaveEditorController::setSceneGraph() {
	deactivateAndClear(_levelEditNode);

    Vec2 screen1;
    Vec2 scene1 = Vec2::Vec2(GAME_SCENE_WIDTH, Util::getSceneToWorldTranslateY());
    auto topBorder = PolygonNode::alloc(Rect::Rect(0,0, scene1.x, scene1.y));
    auto bottomBorder = PolygonNode::alloc(Rect::Rect(0,0, scene1.x, scene1.y));

    topBorder->setColor(Color4::PAPYRUS);
    bottomBorder->setColor(Color4::PAPYRUS);
    _levelEditNode->addChild(topBorder);
    _levelEditNode->addChild(bottomBorder);
    topBorder->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bottomBorder->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    topBorder->setPosition(Vec2::Vec2(0, Util::getGameSceneHeight()-Util::getSceneToWorldTranslateY()));
    bottomBorder->setPosition(Vec2::Vec2(0,0));
    
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
    
    auto refreshButton = Util::makeBoxButton(110, 30, 30, 30, Color4::BLUE, Color4::PAPYRUS);
    refreshButton->setListener(
        [=](const std::string& name, bool down) {
           if (down) {
               _prevState = _state;
               _state = WaveEditorState::REFRESH;
           }
        }
   );

	backButton->activate(getUid());
	addButton->activate(getUid());
    refreshButton->activate(getUid());
	_levelEditNode->addChildWithName(backButton, "back");
	_levelEditNode->addChildWithName(addButton, "add");
    _levelEditNode->addChildWithName(refreshButton, "refresh");
	_levelEditNode->addChildWithName(Node::alloc(), "templates");
    _levelEditNode->addChildWithName(Node::alloc(), "entries");
    
    auto label = Label::alloc(_waveKey, _world->getAssetManager()->get<Font>("Charlemagne_40"));
    label->setScale(0.5);
    label->setPosition(500,550);
    _levelEditNode->addChild(label);
    
    _dragNode = Node::alloc();
    _levelEditNode->addChildWithName(_dragNode, "drag");
    _dragIndex = -1;
}




/* -------------------------------------------------------------- */
/* --------------------- UTILITY STUFF -------------------------- */
/* -------------------------------------------------------------- */

void WaveEditorController::setTemplates(std::vector<std::string> templates){
    _templates.clear();
    for(auto str: templates){
        _templates.push_back(_world->getTemplate(str));
    }
}


void WaveEditorController::setWave(std::string key, std::shared_ptr<WaveData> wave) {
    _currentWave = wave;
    _waveKey = key;
    _state = WaveEditorState::START;
}


std::shared_ptr<TemplateWaveEntry> WaveEditorController::getTemplateWaveEntry(std::string templateKey){
    for (auto t : _templates){
        if (templateKey == t->name) {
            return t;
        }
    }
    
    // the template is not found
    return nullptr;
}


bool WaveEditorController::init(std::shared_ptr<Node> node, std::shared_ptr<World> world) {
	_state = WaveEditorState::START;
	_levelEditNode = node;
	_world = world;
    _colorChanged = false;
    _aiChanged = false;
    _entryRemoved = false;
    _showTemplates = true;
    _zoneChanged = false;
    
    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset("./json/editorLevel.json");
    if (reader == nullptr) {
        CULogError("No asset directory located at");
    }
	std::shared_ptr<JsonValue> json = reader->readJson();
    
    for(std::string tName :json->get("template names")->asStringArray()){
        auto temp = world->getAssetManager()->get<TemplateWaveEntry>(tName);
        _templates.push_back(temp);
    }

    
    //_templates.push_back(world->getAssetManager()->get<TemplateWaveEntry>("kyle0_template"));
    //_templates.push_back(world->getAssetManager()->get<TemplateWaveEntry>("kyle1_template"));
	return true;
}

void WaveEditorController::updateWorld(std::shared_ptr<World> world){
    for (auto templateWaveEntry : _templates){
        world->addTemplate(templateWaveEntry->name,templateWaveEntry);
    }
}
