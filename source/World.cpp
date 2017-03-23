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
	_levelData = LevelData::alloc(1);
	std::mt19937 rng;
	rng.seed(std::random_device()());
	// distribution width and height

	std::uniform_int_distribution<std::mt19937::result_type> dist9(1, 9);
	std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, 31);
	std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, 17);

	for (int i = 0; i < 10; i++) {
		std::shared_ptr<LevelEntry> e = LevelEntry::alloc(dist9(rng), TIME_BETWEEN_SPAWN);
		_levelData->addLevelEntry(e);
	}

	for (int i = 1; i < 10; i++) {
		auto wd = WaveData::alloc(1);
		for (int j = 0; j<NUMBER_SPAWNS; j++) {
			std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
			std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng), distWidth(rng), distHeight(rng), "homing");
			wd->addWaveEntry(we);
		}
		std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
		std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng), distWidth(rng), distHeight(rng), "square");
		wd->addWaveEntry(we);
		_waveData.insert(std::make_pair(i, wd));
	}

	auto od1 = ObjectData::alloc(1, 1, 3, 5, 5, Element::BLUE);
	_objectData.insert(std::make_pair(1, od1));

	auto od2 = ObjectData::alloc(2, 1, 4, 5, 5, Element::GOLD);
	_objectData.insert(std::make_pair(2, od2));

	std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
	_shapeData.insert({ sd->getUID(),sd });

	std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
	_animationData.insert({ blueEnemy->getUID(),blueEnemy });

	std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
	_animationData.insert({ yellowEnemy->getUID(),yellowEnemy });


}

void World::populate_singlefile(){

	// later on, a controller can modify what the current level is, and we load in that level's assets
	string currentLevel = "level1";
	_levelData = _assets->get<LevelData>(currentLevel);

	// iterate though level waves, load the wave data
	for (int i = 1; i < _levelData->getNumberWaves() + 1; i++) 
	{
		string waveKey = "wave" + std::to_string(i);
		_waveData.insert(std::make_pair(i, _assets->get<WaveData>(waveKey)));
	}
	// iterate through wave data, load in objects
	for (int i = 1; i < _waveData.size() + 1; i++)
	{
		for (int j = 1; j < _waveData.at(i)->getWaveEntries().size(); j++)
		{
			int oKey = _waveData.at(i)->getEntry(j)->objectKey;
			string objKey = "object" + std::to_string(oKey);
			auto od = _assets->get<ObjectData>(objKey);
			_objectData.insert(std::make_pair(oKey, od));
		}
	}

	// iterate through object data, load in associated shapes and animation
	for (int i = 1; i < _objectData.size() + 1; i++)
	{
		int sh = _objectData.at(i)->shape_id;
		string shKey = "shape" + std::to_string(sh);
		int anim = _objectData.at(i)->animation_id;
		string animKey = "animation" + std::to_string(anim);
		_shapeData.insert(std::make_pair(sh, _assets->get<ShapeData>(shKey)));
		_animationData.insert(std::make_pair(anim, _assets->get<AnimationData>(animKey)));
	}
    
	// todo: we need to agree on whether or not to use "animation1" instead of "redplayeranimation" as the animation key for consistency
	// for parsing happiness I have currently referenced the enemies with "animation3" and "animation4" for easier lookup via assetmanager.
}

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    populate();
    return true;
}

