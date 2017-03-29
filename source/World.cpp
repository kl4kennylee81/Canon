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

#define TIME_BETWEEN_SPAWN       500
#define NUMBER_SPAWNS            6

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

/** testing function to populate the world without the data files */
void World::populate() {
    this->_isSandbox = true;
	_levelData = LevelData::alloc();
	std::mt19937 rng;
	rng.seed(std::random_device()());
	// distribution width and height

	std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
	std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, GAME_SCENE_WIDTH);
	std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, GAME_SCENE_WIDTH*GAME_SCENE_ASPECT);

	for (int i = 0; i < 10; i++) {
        std::shared_ptr<LevelEntry> e = LevelEntry::alloc("wave"+std::to_string(dist9(rng)), TIME_BETWEEN_SPAWN);
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
		_waveData.insert(std::make_pair("wave"+std::to_string(i), wd));
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

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    // TODO temporary to test if it works
    this->_levelData = assets->get<LevelData>("level0");
    _isSandbox = false;
    
    populate();
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

