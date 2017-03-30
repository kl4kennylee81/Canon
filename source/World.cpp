//
//  World.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "World.hpp"
#include "Element.hpp"
#include <random>
#include "AIData.hpp"
#include "CompositeAIData.hpp"
#include <iostream>
#include <fstream>
#include "StaticZoneData.hpp"
#include "PathAIData.hpp"
#include "AIData.hpp"

#define TIME_BETWEEN_SPAWN       500
#define NUMBER_SPAWNS            6

std::string LEVEL_NAME = "worldSerialize";

using std::string;
using namespace cugl;

World::World() :
_assets(nullptr),
_levelData(nullptr)
{}

World::~World()
{}


std::shared_ptr<GenericAssetManager> World::getAssetManager(){
    return _assets;
}

std::shared_ptr<JsonValue> World::toJsonValue()
{
	std::shared_ptr<JsonValue> completeJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> levelDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> waveDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> objectDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> shapeDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> zoneDataJson = JsonValue::allocObject();

	for (auto const& x : _waveData) { waveDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _objectData) { objectDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _shapeData) { shapeDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _zoneData) { zoneDataJson->appendChild(x.first, x.second->toJsonValue()); }

	levelDataJson->appendChild(LEVEL_NAME, _levelData->toJsonValue());
	
	completeJson->appendChild("levels", levelDataJson);
	completeJson->appendChild("waves", waveDataJson);
	completeJson->appendChild("objects", objectDataJson);
	completeJson->appendChild("shapes", shapeDataJson);
	completeJson->appendChild("zones", zoneDataJson);
	
	return completeJson;
}

std::string World::serialize()
{
	return toJsonValue()->toString();
}

void World::populateLevel0() {
	this->_isSandbox = true;
	_levelData = LevelData::alloc();
	std::mt19937 rng;
	rng.seed(std::random_device()());
	// distribution width and height

	std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
	std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, GAME_SCENE_WIDTH);
	std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, GAME_SCENE_WIDTH*GAME_SCENE_ASPECT);

	for (int i = 0; i < 4; i++) {
		std::shared_ptr<LevelEntry> e = LevelEntry::alloc("kylewave"+std::to_string(i+1), TIME_BETWEEN_SPAWN);
		_levelData->addLevelEntry(e);
	}

	// create player characters
	auto player1 = WaveEntry::alloc("playerChar1", "", 500, 300, Element::BLUE, {});
	_levelData->addPlayerChars(player1);

	auto player1Obj = _assets->get<ObjectData>("playerChar1");
	_objectData.insert({ "playerChar1",player1Obj });

	auto player1Anim = _assets->get<AnimationData>("blueCharAnimation");
	_animationData.insert({ "blueCharAnimation",player1Anim });

	auto player2Obj = _assets->get<ObjectData>("playerChar2");
	_objectData.insert({ "playerChar2",player2Obj });

	auto player2Anim = _assets->get<AnimationData>("redCharAnimation");
	_animationData.insert({ "redCharAnimation",player2Anim });

	auto player2 = WaveEntry::alloc("playerChar2", "", 400, 300, Element::GOLD, {});
	_levelData->addPlayerChars(player2);

	std::shared_ptr<WaveEntry> we;
	auto wd = WaveData::alloc();
	for (int j = 0; j < 8; j++) {
		we = WaveEntry::alloc("object1", "vertical", 200 + (j * 75), distHeight(rng), Element::BLUE, {});
		wd->addWaveEntry(we);
	}
	we = WaveEntry::alloc("object2", "horizontal", 500, 300, Element::GOLD, {"columnZone"});
	wd->addWaveEntry(we);
    
    auto horizontalAILeft = PathAIData::alloc(PathType::HORIZONTAL,{},PathDirection::LEFT);
    _aiData.insert(std::make_pair("horizontalLeft",horizontalAILeft));
    
    we = WaveEntry::alloc("object2","horizontalLeft", 500, 300, Element::GOLD, {"columnZone"});
    wd->addWaveEntry(we);
	_waveData.insert(std::make_pair("kylewave2", wd));

	wd = WaveData::alloc();
	wd->addWaveEntry(WaveEntry::alloc("object2", "homing", 100, 100, Element::GOLD, {}));
	wd->addWaveEntry(WaveEntry::alloc("object2", "homing", 100, 500, Element::GOLD, {}));
	wd->addWaveEntry(WaveEntry::alloc("object2", "homing", 900, 500, Element::GOLD, {}));
	wd->addWaveEntry(WaveEntry::alloc("object2", "homing", 900, 100, Element::GOLD, {}));
	_waveData.insert(std::make_pair("kylewave3", wd));

	wd = WaveData::alloc();
	wd->addWaveEntry(WaveEntry::alloc("object1", "composite", 100, 100, Element::BLUE, {}));
	wd->addWaveEntry(WaveEntry::alloc("object2", "static", 100, 500, Element::GOLD, {"pulseZone"}));
	wd->addWaveEntry(WaveEntry::alloc("object1", "composite", 900, 500, Element::BLUE, {}));
	wd->addWaveEntry(WaveEntry::alloc("object2", "static", 900, 100, Element::GOLD, {"rotateZone"}));
	_waveData.insert(std::make_pair("kylewave4", wd));

	auto od1 = ObjectData::alloc("shape1", "blueEnemyAnimation");
	_objectData.insert(std::make_pair("object1", od1));

	auto od2 = ObjectData::alloc("shape1", "redEnemyAnimation");
	_objectData.insert(std::make_pair("object2", od2));

	std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
	_shapeData.insert({ "shape1",sd });

	std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
	_animationData.insert({ "blueEnemyAnimation",blueEnemy });

	std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
	_animationData.insert({ "redEnemyAnimation",yellowEnemy });

	std::shared_ptr<AIData> homingAI = _assets->get<AIData>("homing");
	_aiData.insert({ "homing",homingAI });
	std::shared_ptr<AIData> squareAI = _assets->get<AIData>("square");
	_aiData.insert({ "square",squareAI });
	std::shared_ptr<AIData> verticalAI = _assets->get<AIData>("vertical");
	_aiData.insert({ "vertical",verticalAI });


	std::shared_ptr<ZoneData> staticZone = _assets->get<ZoneData>("staticZone");
	_zoneData.insert({ "staticZone", staticZone });
	std::shared_ptr<ZoneData> columnZone = _assets->get<ZoneData>("columnZone");

	auto zone = std::static_pointer_cast<StaticZoneData>(columnZone);
	std::string el = zone->element == Element::BLUE ? "BLUE" : "GOLD";

	_zoneData.insert({ "columnZone", columnZone });    

    // hong stuff
    auto redBigCircleZone = StaticZoneData::alloc("big_circle", 0, 0,  0, 100, Element::GOLD);
    _zoneData.insert({"redBigCircleZone", redBigCircleZone });
    
    std::shared_ptr<LevelEntry> e = LevelEntry::alloc("kylewave3", TIME_BETWEEN_SPAWN);
    _levelData->addLevelEntry(e);
    
    // add wave entries here
    auto we1 = WaveEntry::alloc("object1", "horizontal", 0, 100,Element::BLUE,{"staticZoneBig"});
    
    auto we2 = WaveEntry::alloc("object2", "horizontal", 940, 440,Element::GOLD,{"redBigCircleZone"});
    
    auto we3 = WaveEntry::alloc("object2", "vertical", 0, 100,Element::GOLD,{"redBigCircleZone"});
    
    auto we4 = WaveEntry::alloc("object1", "vertical", 940, 440,Element::BLUE,{"staticZoneBig"});

    wd = WaveData::alloc();
    wd->addWaveEntry(we1);
    wd->addWaveEntry(we2);
    wd->addWaveEntry(we3);
    wd->addWaveEntry(we4);
    
    std::shared_ptr<WaveEntry> we5;
    for (int j = 0; j<NUMBER_SPAWNS; j++) {
        std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
        if (dist2(rng) == 1){
            we5 = WaveEntry::alloc("object1", "homing", distWidth(rng), distHeight(rng),Element::BLUE,{});
        } else {
            we5 = WaveEntry::alloc("object2", "homing", distWidth(rng), distHeight(rng),Element::GOLD,{});
        }
        wd->addWaveEntry(we5);
    }
    
    // add the wave entry to the wave data
    _waveData.insert(std::make_pair("kylewave1", wd));

}

void World::populateLevel1() {
    this->_isSandbox = true;
	_levelData = LevelData::alloc();
	std::mt19937 rng;
	rng.seed(std::random_device()());
	// distribution width and height

	std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
	std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, GAME_SCENE_WIDTH);
	std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, GAME_SCENE_WIDTH*GAME_SCENE_ASPECT);

	for (int i = 0; i < 10; i++) {
        std::shared_ptr<LevelEntry> e = LevelEntry::alloc("level1wave"+std::to_string(dist9(rng)), TIME_BETWEEN_SPAWN);
		_levelData->addLevelEntry(e);
	}
    
    // create player characters
    auto player1 = WaveEntry::alloc("playerChar1", "", 500, 200, Element::BLUE, {});
    _levelData->addPlayerChars(player1);
    
    auto player1Obj = _assets->get<ObjectData>("playerChar1");
    _objectData.insert({"playerChar1",player1Obj});
    
    auto player1Anim = _assets->get<AnimationData>("blueCharAnimation");
    _animationData.insert({ "blueCharAnimation",player1Anim });
    
    auto player2Obj = _assets->get<ObjectData>("playerChar2");
    _objectData.insert({"playerChar2",player2Obj});
    
    auto player2Anim = _assets->get<AnimationData>("redCharAnimation");
    _animationData.insert({ "redCharAnimation",player2Anim });
    
    auto player2 = WaveEntry::alloc("playerChar2", "", 550, 250, Element::GOLD, {});
    _levelData->addPlayerChars(player2);

    std::shared_ptr<WaveEntry> we;
	for (int i = 1; i < 10; i++) {
		auto wd = WaveData::alloc();
		for (int j = 0; j<NUMBER_SPAWNS; j++) {
			std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
            if (dist2(rng) == 1){
                we = WaveEntry::alloc("object1", "vertical", distWidth(rng), distHeight(rng),Element::BLUE,{"staticZone"});
            } else {
                we = WaveEntry::alloc("object2", "homing", distWidth(rng), distHeight(rng),Element::GOLD,{});
            }
			wd->addWaveEntry(we);
		}
		_waveData.insert(std::make_pair("level1wave"+std::to_string(i), wd));
	}
    

	auto od1 = ObjectData::alloc("shape1","blueEnemyAnimation");
	_objectData.insert(std::make_pair("object1", od1));

    auto od2 = ObjectData::alloc("shape1", "redEnemyAnimation");
	_objectData.insert(std::make_pair("object2", od2));

	std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
	_shapeData.insert({"shape1",sd });

	std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
	_animationData.insert({ "blueEnemyAnimation",blueEnemy });

	std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
	_animationData.insert({ "redEnemyAnimation",yellowEnemy });
    
	std::shared_ptr<AIData> homingAI = _assets->get<AIData>("homing");
    _aiData.insert({"homing",homingAI});
	std::shared_ptr<AIData> squareAI = _assets->get<AIData>("square");
    _aiData.insert({"square",squareAI});
    

    std::shared_ptr<ZoneData> staticZone = _assets->get<ZoneData>("staticZone");
    _zoneData.insert({"staticZone", staticZone});
    
}

void World::populateLevel2() {
    this->_isSandbox = true;
    
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // distribution width and height
    
    std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
    std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, GAME_SCENE_WIDTH);
    std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, GAME_SCENE_WIDTH*GAME_SCENE_ASPECT);

    
    _levelData = LevelData::alloc();
    
    // level data for players
    
    auto player1 = WaveEntry::alloc("playerChar1", "", 500, 200, Element::BLUE, {});
    _levelData->addPlayerChars(player1);
    
    auto player1Obj = _assets->get<ObjectData>("playerChar1");
    _objectData.insert({"playerChar1",player1Obj});
    
    auto player1Anim = _assets->get<AnimationData>("blueCharAnimation");
    _animationData.insert({ "blueCharAnimation",player1Anim });
    
    auto player2Obj = _assets->get<ObjectData>("playerChar2");
    _objectData.insert({"playerChar2",player2Obj});
    
    auto player2Anim = _assets->get<AnimationData>("redCharAnimation");
    _animationData.insert({ "redCharAnimation",player2Anim });
    
    auto player2 = WaveEntry::alloc("playerChar2", "", 550, 250, Element::GOLD, {});
    _levelData->addPlayerChars(player2);
    
    // other data
    
    auto od1 = ObjectData::alloc("shape1","blueEnemyAnimation");
    _objectData.insert(std::make_pair("object1", od1));
    
    auto od2 = ObjectData::alloc("shape1", "redEnemyAnimation");
    _objectData.insert(std::make_pair("object2", od2));
    
    std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
    _shapeData.insert({"shape1",sd });
    
    std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
    _animationData.insert({ "blueEnemyAnimation",blueEnemy });
    
    std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
    _animationData.insert({ "redEnemyAnimation",yellowEnemy });
    
    std::shared_ptr<AIData> homingAI = _assets->get<AIData>("homing");
    _aiData.insert({"homing",homingAI});
    std::shared_ptr<AIData> squareAI = _assets->get<AIData>("square");
    _aiData.insert({"square",squareAI});
    
    std::shared_ptr<ZoneData> staticZone = _assets->get<ZoneData>("staticZone");
    
    _zoneData.insert({"staticZone", staticZone});
    
    auto redBigCircleZone = StaticZoneData::alloc("big_circle", 0, 0,  0, 100, Element::GOLD);
    _zoneData.insert({"redBigCircleZone", redBigCircleZone });
    
    std::shared_ptr<LevelEntry> e = LevelEntry::alloc(LEVEL_NAME + "wave0", TIME_BETWEEN_SPAWN);
    _levelData->addLevelEntry(e);
    
    auto wd = WaveData::alloc();
    
    // add wave entries here
    auto we = WaveEntry::alloc("object1", "horizontal", 0, 100,Element::BLUE,{"staticZoneBig"});
    
    auto we2 = WaveEntry::alloc("object2", "horizontal", 940, 440,Element::GOLD,{"redBigCircleZone"});
    
    auto we3 = WaveEntry::alloc("object2", "vertical", 0, 100,Element::GOLD,{"redBigCircleZone"});
    
    auto we4 = WaveEntry::alloc("object1", "vertical", 940, 440,Element::BLUE,{"staticZoneBig"});

    wd->addWaveEntry(we);
    wd->addWaveEntry(we2);
    wd->addWaveEntry(we3);
    wd->addWaveEntry(we4);
    
    std::shared_ptr<WaveEntry> we5;
    for (int j = 0; j<NUMBER_SPAWNS; j++) {
        std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
        if (dist2(rng) == 1){
            we5 = WaveEntry::alloc("object1", "homing", distWidth(rng), distHeight(rng),Element::BLUE,{});
        } else {
            we5 = WaveEntry::alloc("object2", "homing", distWidth(rng), distHeight(rng),Element::GOLD,{});
        }
        wd->addWaveEntry(we5);
    }
    
    // add the wave entry to the wave data
    _waveData.insert(std::make_pair(LEVEL_NAME + "wave0", wd));
}

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    // TODO temporary to test if it works
	LEVEL_NAME = "level0";

    this->_levelData = assets->get<LevelData>(LEVEL_NAME);
    _isSandbox = false;
    
    //populateLevel0();

	//std::ofstream myfile;
	//myfile.open("json/kylelevel0a.json");
	//myfile << this->serialize();
	//myfile.close();
    return true;
}

std::shared_ptr<ObjectData> World::getObjectData(std::string obKey){
    if (_isSandbox && _objectData.count(obKey) > 0){
        return _objectData.at(obKey);
    }
    // default to the asset if can't find in sandbox
    return _assets->get<ObjectData>(obKey);
}

std::shared_ptr<AnimationData> World::getAnimationData(std::string aKey){
    if (_isSandbox && _animationData.count(aKey) > 0){
        return _animationData.at(aKey);
    }
    return _assets->get<AnimationData>(aKey);
}

std::shared_ptr<PathData> World::getPathData(std::string pathKey){
    if (_isSandbox && _pathData.count(pathKey) > 0){
        return _pathData.at(pathKey);
    }
    return _assets->get<PathData>(pathKey);
}

std::shared_ptr<ShapeData> World::getShapeData(std::string shapeKey){
    if (_isSandbox && _shapeData.count(shapeKey) > 0){
        return _shapeData.at(shapeKey);
    }
    return _assets->get<ShapeData>(shapeKey);
}

std::shared_ptr<WaveData> World::getWaveData(std::string waveKey){
    if (_isSandbox && _waveData.count(waveKey) > 0){
        return _waveData.at(waveKey);
    }
    return _assets->get<WaveData>(waveKey);
}

std::shared_ptr<AIData> World::getAIData(std::string aiKey){
	bool sandbox = _isSandbox && _aiData.count(aiKey) > 0;
	auto data = sandbox ? _aiData[aiKey] : _assets->get<AIData>(aiKey);
	if (data != nullptr && data->type == AIType::COMPOSITE) {
		auto compositeData = std::static_pointer_cast<CompositeAIData>(data);
		std::string startKey = compositeData->_startKey;
		compositeData->_startData = sandbox ? _aiData[startKey] : _assets->get<AIData>(startKey);
		for (int i = 0; i < compositeData->_aiKeys.size(); i++) {
			std::shared_ptr<AIData> subData = getAIData(compositeData->_aiKeys.at(i));
			compositeData->_aiDatas.push_back(subData);
		}
		return compositeData;
	}
	return data;
}

std::shared_ptr<ZoneData> World::getZoneData(std::string zoneKey){
    if (_isSandbox && _zoneData.count(zoneKey) > 0){
        return _zoneData.at(zoneKey);
    }
    return _assets->get<ZoneData>(zoneKey);
}

