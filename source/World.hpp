//
//  World.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_map>
#include "ObjectData.hpp"
#include "AnimationData.hpp"
#include "ShapeData.hpp"
#include "WaveData.hpp"
#include "BulletData.hpp"
#include "LevelData.hpp"
#include "ZoneData.hpp"
#include "SoundData.hpp"
#include "GenericAssetManager.hpp"
#include "AIData.hpp"
#include "TemplateWaveEntry.hpp"
#include "ParticleStateData.hpp"

/** contain all the static data loaded in metadata needed/ prototypes of
  * path data, physics shape data, animation data etc. to spawn out the active
  * version in game
  *
  * ex. shape data needed to create a box2d Obstacle
  * ex. path data for the prespecified movement to generate the active path 
  *     in the the enemy AI controller

  */

class World {
protected:
    // the level of the world
    std::shared_ptr<LevelData> _levelData;
    
    /** The asset manager for this game world. */
    std::shared_ptr<GenericAssetManager> _assets;

public:
    /** if this is a sandbox prepopulated instance */
    bool _isSandbox;
    
    // static prototypes used to spawn units into the gameState
    std::unordered_map<std::string, std::shared_ptr<ObjectData>> _objectData;
    std::unordered_map<std::string, std::shared_ptr<AnimationData>> _animationData;
    std::unordered_map<std::string, std::shared_ptr<ShapeData>> _shapeData;
    std::unordered_map<std::string, std::shared_ptr<WaveData>> _waveData;
    std::unordered_map<std::string, std::shared_ptr<AIData>> _aiData;
    std::unordered_map<std::string, std::shared_ptr<ZoneData>> _zoneData;
    std::unordered_map<std::string, std::shared_ptr<SoundData>> _soundData;
    std::unordered_map<std::string, std::shared_ptr<TemplateWaveEntry>> _templateData;
    std::unordered_map<std::string, std::shared_ptr<BulletData>> _bulletData;
    std::unordered_map<std::string, std::shared_ptr<ParticleStateData>> _partStateData;
    
    std::unordered_set<std::string> _tempKeys;

    World();
    
    ~World();
    
    virtual bool init(std::shared_ptr<GenericAssetManager> assets){
        _assets = assets;
        return true;
    }

    virtual bool init(std::shared_ptr<GenericAssetManager> assets, std::string levelName);
    
    virtual bool init(std::shared_ptr<GenericAssetManager> assets, std::shared_ptr<LevelData> levelData){
        _assets = assets;
        _levelData = levelData;
        return true;
    }

    static std::shared_ptr<World> alloc(std::shared_ptr<GenericAssetManager> assets) {
		std::shared_ptr<World> result = std::make_shared<World>();
		return (result->init(assets) ? result : nullptr);
	}

    static std::shared_ptr<World> alloc(std::shared_ptr<GenericAssetManager> assets,std::string levelName) {
        std::shared_ptr<World> result = std::make_shared<World>();
        return (result->init(assets,levelName) ? result : nullptr);
    }
    
    static std::shared_ptr<World> alloc(std::shared_ptr<GenericAssetManager> assets,std::shared_ptr<LevelData> levelData) {
        std::shared_ptr<World> result = std::make_shared<World>();
        return (result->init(assets,levelData) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::JsonValue> toJsonValue(std::string levelName);
    
    std::string serialize(std::string levelName);
    
    std::shared_ptr<GenericAssetManager> getAssetManager();
    
    std::shared_ptr<ObjectData> getObjectData(std::string obKey);
    
    std::shared_ptr<AnimationData> getAnimationData(std::string aKey);
    
    std::shared_ptr<ShapeData> getShapeData(std::string shapeKey);
    
    std::shared_ptr<WaveData> getWaveData(std::string waveKey);
    
    std::shared_ptr<AIData> getAIData(std::string aiKey);
    
    std::shared_ptr<BulletData> getBulletData(std::string bulletKey);
    
    std::shared_ptr<ZoneData> getZoneData(std::string zoneKey);

    std::shared_ptr<SoundData> getSoundData(std::string soundKey);
    
    std::shared_ptr<ParticleStateData> getParticleStateData(std::string partKey);
    
    std::shared_ptr<TemplateWaveEntry> getTemplate(std::string templateKey);
    
    std::shared_ptr<ObjectData> getObjectData(std::shared_ptr<WaveEntry> we);
    
    std::shared_ptr<AIData> getAIData(std::shared_ptr<WaveEntry> we);
    
    std::vector<std::string> getZoneKeys(std::shared_ptr<WaveEntry> we);
    
    std::shared_ptr<BulletData> getBulletData(std::shared_ptr<WaveEntry> we);
    
    std::shared_ptr<LevelData> getLevelData(){
        return _levelData;
    }
    
    void setLevelData(std::shared_ptr<LevelData> levelData){
        _levelData = levelData;
    }
    
    void setLevelData(std::string levelName){
        _levelData = _assets->get<LevelData>(levelName);
    }
    
    void addTemplate(std::string templateKey, std::shared_ptr<TemplateWaveEntry>);

    void populate();
    
    bool isValid();
    	
    void copyWave(std::string copiedWaveKey, std::string newWaveKey);
    
};

#endif /* World_hpp */
