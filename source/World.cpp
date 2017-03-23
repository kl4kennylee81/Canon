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
#include <iostream>
#include <fstream>

#define TIME_BETWEEN_SPAWN       500
#define NUMBER_SPAWNS            5

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
//	_levelData = LevelData::alloc(1);
//	std::mt19937 rng;
//	rng.seed(std::random_device()());
//	// distribution width and height
//
//	std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
//	std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, 31);
//	std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, 17);
//
//	for (int i = 0; i < 10; i++) {
//		std::shared_ptr<LevelEntry> e = LevelEntry::alloc(dist9(rng), TIME_BETWEEN_SPAWN);
//		_levelData->addLevelEntry(e);
//	}
//
//	for (int i = 1; i < 10; i++) {
//		auto wd = WaveData::alloc(1);
//		for (int j = 0; j<NUMBER_SPAWNS; j++) {
//			std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
//			auto ai = _assets->get<AIData>("homing");
//			std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng), distWidth(rng), distHeight(rng), ai);
//			wd->addWaveEntry(we);
//		}
//		std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
//		auto ai = _assets->get<AIData>("square");
//		std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng), distWidth(rng), distHeight(rng), ai);
//		wd->addWaveEntry(we);
//		_waveData.insert(std::make_pair(i, wd));
//	}
//
//	auto od1 = ObjectData::alloc(1, 1, 3, 5, 5, Element::BLUE);
//	_objectData.insert(std::make_pair(1, od1));
//
//	auto od2 = ObjectData::alloc(2, 1, 4, 5, 5, Element::GOLD);
//	_objectData.insert(std::make_pair(2, od2));
//
//	std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
//	_shapeData.insert({ sd->getUID(),sd });
//
//	std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
//	_animationData.insert({ blueEnemy->getUID(),blueEnemy });
//
//	std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
//	_animationData.insert({ yellowEnemy->getUID(),yellowEnemy });
//
//
}

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    // TODO temporary to test if it works
    this->_levelData = assets->get<LevelData>("level0");
    this->_isSandbox = false;
    
    //    populate();
    return true;
}

std::shared_ptr<ObjectData> World::getObjectData(std::string obKey){
    if (_isSandbox){
        return _objectData.at(obKey);
    }
    return _assets->get<ObjectData>(obKey);
}

std::shared_ptr<AnimationData> World::getAnimationData(std::string aKey){
    if (_isSandbox){
        return _animationData.at(aKey);
    }
    return _assets->get<AnimationData>(aKey);
}

std::shared_ptr<PathData> World::getPathData(std::string pathKey){
    if (_isSandbox){
        return _pathData.at(pathKey);
    }
    return _assets->get<PathData>(pathKey);
}

std::shared_ptr<ShapeData> World::getShapeData(std::string shapeKey){
    if (_isSandbox){
        return _shapeData.at(shapeKey);
    }
    return _assets->get<ShapeData>(shapeKey);
}

std::shared_ptr<WaveData> World::getWaveData(std::string waveKey){
    if (_isSandbox){
        return _waveData.at(waveKey);
    }
    return _assets->get<WaveData>(waveKey);
}

std::shared_ptr<AIData> World::getAIData(std::string aiKey){
    if (_isSandbox){
        return _aiData.at(aiKey);
    }
    return _assets->get<AIData>(aiKey);
}

