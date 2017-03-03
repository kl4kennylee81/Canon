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
    // distribution width and height
    std::uniform_int_distribution<std::mt19937::result_type> distWidth(1,Application::get()->getDisplayWidth());
    std::uniform_int_distribution<std::mt19937::result_type> distHeight(1,Application::get()->getDisplayHeight());
    
    auto wd = WaveData::alloc(1);
    for (int i = 0;i<4;i++){
        WaveEntry we = WaveEntry(1,distWidth(rng),distHeight(rng));
        wd->addWaveEntry(we);
    }
    
    _waveData.insert(std::make_pair(1,wd));
    
    std::uniform_int_distribution<std::mt19937::result_type> dist2(1,2);
    Element element;
    if (dist2(rng) == 1){
        element = Element::BLUE;
    } else {
        element = Element::GOLD;
    }
    
    auto od = ObjectData::alloc();
    od->init(1,1,5,5,element);
    _objectData.insert(std::make_pair(1,od));
    
    auto sd = ShapeData::alloc(1,50,50);
    _shapeData.insert(std::make_pair(1,sd));
}

bool World::init(std::shared_ptr<cugl::AssetManager> assets){
    _assets = assets;
    populate();
    return true;
}

