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

#define TIME_BETWEEN_SPAWN       500
#define NUMBER_SPAWNS            3


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
    _levelData = LevelData::alloc(1);
    std::mt19937 rng;
    rng.seed(std::random_device()());
    // distribution width and height
    
    std::uniform_int_distribution<std::mt19937::result_type> dist9(1,9);
    std::uniform_int_distribution<std::mt19937::result_type> distWidth(1,Application::get()->getDisplayWidth());
    std::uniform_int_distribution<std::mt19937::result_type> distHeight(1,Application::get()->getDisplayHeight());
    
    for (int i = 0;i < 10;i++){
        std::shared_ptr<LevelEntry> e = LevelEntry::alloc(dist9(rng),TIME_BETWEEN_SPAWN);
        _levelData->addLevelEntry(e);
    }
    
    for (int i = 1;i < 10;i++){
        auto wd = WaveData::alloc(1);
        for (int j = 0;j<NUMBER_SPAWNS;j++){
            std::uniform_int_distribution<std::mt19937::result_type> dist2(1,2);
            std::shared_ptr<WaveEntry> we = WaveEntry::alloc(dist2(rng),distWidth(rng),distHeight(rng));
            wd->addWaveEntry(we);
        }
        _waveData.insert(std::make_pair(i,wd));
    }
    
    auto od1 = ObjectData::alloc(1,1,5,5,Element::BLUE);
    _objectData.insert(std::make_pair(1,od1));
    
    auto od2 = ObjectData::alloc(2,1,5,5,Element::GOLD);
    _objectData.insert(std::make_pair(2,od2));
    
    auto sd = ShapeData::alloc(1,50,50);
    _shapeData.insert(std::make_pair(1,sd));
    
}

bool World::init(std::shared_ptr<GenericAssetManager> assets){
    _assets = assets;
    populate();
    return true;
}

