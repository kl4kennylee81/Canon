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
#include "GameEngine.hpp"

#define TIME_BETWEEN_SPAWN       500
#define NUMBER_SPAWNS            4

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

std::shared_ptr<JsonValue> World::toJsonValue(std::string levelName)
{
    // TODO need to update world serialize
	std::shared_ptr<JsonValue> completeJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> levelDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> waveDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> objectDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> shapeDataJson = JsonValue::allocObject();
	std::shared_ptr<JsonValue> zoneDataJson = JsonValue::allocObject();
    std::shared_ptr<JsonValue> animationDataJson = JsonValue::allocObject();

	for (auto const& x : _waveData) { waveDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _objectData) { objectDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _shapeData) { shapeDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _zoneData) { zoneDataJson->appendChild(x.first, x.second->toJsonValue()); }
	for (auto const& x : _animationData) { animationDataJson->appendChild(x.first, x.second->toJsonValue()); }
    
	levelDataJson->appendChild(levelName, _levelData->toJsonValue());
	
	completeJson->appendChild("levels", levelDataJson);
	completeJson->appendChild("waves", waveDataJson);
	completeJson->appendChild("objects", objectDataJson);
	completeJson->appendChild("shapes", shapeDataJson);
	completeJson->appendChild("zones", zoneDataJson);
    completeJson->appendChild("animations", animationDataJson);
	
	return completeJson;
}

std::string World::serialize(std::string levelName)
{
	return toJsonValue(levelName)->toString();
}

void World::populate() {
}

bool World::init(std::shared_ptr<GenericAssetManager> assets, std::string levelName){
    _assets = assets;
    this->_levelData = assets->get<LevelData>(levelName);
    _isSandbox = false;
    
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

std::shared_ptr<TemplateWaveEntry> World::getTemplate(std::string templateKey){
    if (_isSandbox && _templateData.count(templateKey) > 0){
        return _templateData.at(templateKey);
    }
    return _assets->get<TemplateWaveEntry>(templateKey);
}

std::shared_ptr<ObjectData> World::getObjectData(std::shared_ptr<WaveEntry> we){
    std::shared_ptr<TemplateWaveEntry> templData = getTemplate(we->getTemplateKey());
    if (templData == nullptr) {
        return nullptr;
    }
    return getObjectData(templData->getObjectKey());
}

std::shared_ptr<AIData> World::getAIData(std::shared_ptr<WaveEntry> we){
    return getAIData(we->getAIKey());
}

std::shared_ptr<BulletData> World::getBulletData(std::shared_ptr<WaveEntry> we){
    std::shared_ptr<TemplateWaveEntry> templData = getTemplate(we->getTemplateKey());
    if(templData == nullptr){
        return nullptr;
    }
    return getBulletData(templData->bulletKey);
}


std::vector<std::string> World::getZoneKeys(std::shared_ptr<WaveEntry> we){
    std::shared_ptr<TemplateWaveEntry> templData = getTemplate(we->getTemplateKey());
    if (templData == nullptr) {
        return {};
    }
    return templData->getZoneKeys();
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

std::shared_ptr<BulletData> World::getBulletData(std::string bulletKey){
    if (_isSandbox && _bulletData.count(bulletKey) > 0){
        return _bulletData.at(bulletKey);
    }
    return _assets->get<BulletData>(bulletKey);
}

std::shared_ptr<SoundData> World::getSoundData(std::string soundKey){
    if (_isSandbox && _soundData.count(soundKey) > 0){
        return _soundData.at(soundKey);
    }
    return _assets->get<SoundData>(soundKey);
}

std::shared_ptr<ParticleStateData> World::getParticleStateData(std::string partKey){
    if (_isSandbox && _partStateData.count(partKey) > 0){
        return _partStateData.at(partKey);
    }
    return _assets->get<ParticleStateData>(partKey);
}

void World::addTemplate(std::string templateKey, std::shared_ptr<TemplateWaveEntry> twe){
    _templateData.insert(std::make_pair(templateKey,twe));
}

bool World::isValid(){
    if (_levelData == nullptr){
        return false;
    }
    
    if (_assets == nullptr) {
        return false;
    }
    
    return _levelData->isValid();
}


void World::copyWave(std::string copiedWaveKey, std::string newWaveKey){
    auto newWaveData = WaveData::alloc(getWaveData(copiedWaveKey));
    _waveData.at(newWaveKey) = newWaveData;
}

