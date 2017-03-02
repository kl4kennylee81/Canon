//
//  World.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "World.hpp"
#include <random>


using namespace cugl;

World::World() :
_assets(nullptr),
_levelData(nullptr)
{}

World::~World()
{}


std::shared_ptr<cugl::AssetManager> World::getAssetManager(){
    return _assets;
}

/** testing function to populate the world without the data files */
void World::populate(){
    _levelData = LevelData::alloc(1);
    for (int i = 0;i<10;i++){
        LevelEntry e = LevelEntry(1,i*10);
        _levelData->addLevelEntry(e);
    }
    
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);
    
    auto wd = WaveData::alloc(1);
    for (int i = 0;i<4;i++){
        WaveEntry we = WaveEntry(1,dist100(rng),dist100(rng));
        wd->addWaveEntry(we);
    }
    
    _waveData.insert(std::make_pair(1,wd));
    
    auto od = ObjectData::alloc();
    od->init(1,1,5,5);
    _objectData.insert(std::make_pair(1,od));
    
    auto sd = ShapeData::alloc(1,50,50);
    _shapeData.insert(std::make_pair(1,sd));
}

