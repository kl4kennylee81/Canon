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
void World::populate(){

	// later on, a controller will modify what the current level is, and the assetmanager will load its assets appropriately
	string currentLevel = "level1";
	//_levelData = _assets->get<LevelData>(currentLevel);

    _levelData = LevelData::alloc(1);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // distribution width and height
    
    std::uniform_int_distribution<std::mt19937::result_type> dist9(1,9);
    std::uniform_int_distribution<std::mt19937::result_type> distWidth(0,31);
    std::uniform_int_distribution<std::mt19937::result_type> distHeight(0,17);
    
    for (int i = 0;i < 10;i++){
        std::shared_ptr<LevelEntry> e = LevelEntry::alloc(dist9(rng),TIME_BETWEEN_SPAWN);
        _levelData->addLevelEntry(e);
    }
	
	std::ofstream myfile; // file stuff
	myfile.open("tempWavesLol.json"); // file stuff
	myfile << "{";

    for (int i = 1; i < 10; i++) {
		if (i > 1) myfile << ","; // file stuff

		// write wave json to file (TEMPORARY) for ease

		myfile << "\"wave" << std::to_string(i) << "\":" << "{"; // file stuff

        auto wd = WaveData::alloc(1);
        for (int j = 0;j<NUMBER_SPAWNS;j++){
			
			if (j > 0) myfile << ","; // file stuff

            std::uniform_int_distribution<std::mt19937::result_type> dist2(1,2);
			auto ai = _assets->get<AIData>("homing");

			int dist = dist2(rng); int width = distWidth(rng); int height = distHeight(rng);

            std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist, width, height, ai);
            wd->addWaveEntry(we);

			myfile << "\"object" << std::to_string(j) << "\":" << "{"; // file stuff begin
			myfile << "\"objectKey\":" << std::to_string(dist) << "," << 
				"\"x\":" << std::to_string(width) << "," << 
				"\"y\":" << std::to_string(height) << "," <<
				"\"aiType\":" << "\"homing\"" << "," <<
				"\"pathType\":" << "\"none\"" << "}"; // file stuff end
	
        }
		myfile << "}"; // file stuff
		

		std::uniform_int_distribution<std::mt19937::result_type> dist2(1, 2);
		auto ai = _assets->get<AIData>("square");
		std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng), distWidth(rng), distHeight(rng), ai);

		wd->addWaveEntry(we);
        _waveData.insert(std::make_pair(i,wd));

    }

	myfile << "}"; // file stuff
	myfile.close(); // file stuff
    
    auto od1 = ObjectData::alloc(1,1,3,5,5,Element::BLUE);
    _objectData.insert(std::make_pair(1,od1));
    
    auto od2 = ObjectData::alloc(2,1,4,5,5,Element::GOLD);
    _objectData.insert(std::make_pair(2,od2));
    
    std::shared_ptr<ShapeData> sd = _assets->get<ShapeData>("shape1");
    _shapeData.insert({sd->getUID(),sd});
    
    std::shared_ptr<AnimationData> blueEnemy = _assets->get<AnimationData>("blueEnemyAnimation");
    _animationData.insert({blueEnemy->getUID(),blueEnemy});
    
    std::shared_ptr<AnimationData> yellowEnemy = _assets->get<AnimationData>("redEnemyAnimation");
    _animationData.insert({yellowEnemy->getUID(),yellowEnemy});
    
    
}

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    populate();
    return true;
}

