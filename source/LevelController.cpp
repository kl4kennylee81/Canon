//
//  LevelController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelController.hpp"
#include "LevelEvent.hpp"
#include <math.h>

using namespace cugl;

LevelController::LevelController():
BaseController(),
_world(nullptr){}

void LevelController::attach(Observer* obs) {
    BaseController::attach(obs);
}
void LevelController::detach(Observer* obs) {
    BaseController::detach(obs);
}
void LevelController::notify(Event* e) {
    BaseController::notify(e);
}

/**
 * Update the observer state based on an event from the subject
 */
void LevelController::eventUpdate(Event* e) {}

void LevelController::spawnWaveEntry(std::shared_ptr<WaveEntry> we, bool isPlayer,std::shared_ptr<GameState> state){
    std::shared_ptr<TemplateWaveEntry> templated = _world->getTemplate(we->getTemplateKey());
    std::shared_ptr<ObjectData> od = _world->getObjectData(we);
    std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
    std::shared_ptr<AnimationData> animationd = _world->getAnimationData(od->getAnimationKey(we->getElement()));
    std::shared_ptr<AIData> aid = _world->getAIData(we); // aiKey is in the wave entry
    std::shared_ptr<SoundData> sounddata = _world->getSoundData(od->getSoundKey());
    if (sounddata == nullptr){
        sounddata = _world->getSoundData("baseEnemySound");
    }
    std::vector<std::shared_ptr<ZoneData>> zds = {};
    for (std::string zoneKey:_world->getZoneKeys(we)){
        zds.push_back(_world->getZoneData(zoneKey));
    }

    
    std::shared_ptr<GameObject> gameOb = GameObject::alloc();
    
    // map the uid of the gameObject to the waveEntry key used to identify it in the json (from the parent class data)
    _uidToWaveEntryMap.insert(std::make_pair(gameOb->getUid(),we->key));
    
    gameOb->setIsPlayer(isPlayer);
    
    std::shared_ptr<ObjectInitEvent> initevent = ObjectInitEvent::alloc(gameOb, we, od, animationd, sd, sounddata, aid, zds);
    notify(initevent.get());
    
    if (isPlayer){
        // player is added to the game state here
        state->addPlayerGameObject(gameOb);
    } else {
        // enemy is added to the game state here
        state->addEnemyGameObject(gameOb);
    }
    
    std::shared_ptr<ObjectSpawningEvent> spawningevent = ObjectSpawningEvent::alloc(gameOb,templated->getSpawnTime());
    
    // notify the observers of the object that is spawned
    notify(spawningevent.get());
}

void LevelController::dispose(){
    _world = nullptr;
    _progressBarController = nullptr;
}

void LevelController::update(float timestep,std::shared_ptr<GameState> state){
    
    // send the player spawning event
    if (!_level.hasPlayerSpawned()){
        _level.togglePlayerSpawned();
        for (auto playerEntry : _level.getPlayerChars()){
            spawnWaveEntry(playerEntry,true,state);
        }
    }
    
    _level.update(timestep);
    _progressBarController->update(state,_level);
    if (_level.isReadyToSpawn()){
        _level.toggleReadyToSpawn();
        std::string waveKey = _level.getCurrentWaveKey();
        // spawn the gameObject from the prototypes
        std::shared_ptr<WaveData> wd = _world->getWaveData(waveKey);
        for(auto it: wd->getWaveEntries()) {
            spawnWaveEntry(it, false, state);
        }
    }
    
    if (_level.isSpawningFinished()){
        std::shared_ptr<LevelFinishedEvent> levelFEvent = LevelFinishedEvent::alloc();
        this->notify(levelFEvent.get());
    }
}

bool LevelController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
    _world = world;
    _level.init(world->getLevelData());
    _progressBarController = ProgressBarController::alloc(state,world);
    return true;
}

bool LevelController::init(std::string levelDataKey, std::shared_ptr<GameState> state, std::shared_ptr<World> world){
    // set the level data of the world after resuming
    _world->setLevelData(_world->getAssetManager()->get<LevelData>(levelDataKey));
    _level.init(_world->getLevelData());
    _progressBarController = ProgressBarController::alloc(state,world);
    return true;
}

void LevelController::initAfterResume(std::shared_ptr<GameState> state,
                                      std::shared_ptr<cugl::JsonValue> levelControlJson,
                                      std::shared_ptr<cugl::JsonValue> spawnControlJson)
{
    // set the level data of the world after resuming
    init(levelControlJson->getString("levelDataKey"),state,_world);
    
    // update the level based on what was saved in the levelControlJson
    _level.init(levelControlJson);
    
    std::shared_ptr<JsonValue> enemylist = levelControlJson->get("enemyObjects");
    for (int i = 0; i < enemylist->size(); i++) {
        auto resumeEnemy = enemylist->get(i);
        
        // info from the serialized active objects of players
        std::string waveId = resumeEnemy->getString("waveID");
        float xPos = resumeEnemy->getFloat("currentPosX");
        float yPos = resumeEnemy->getFloat("currentPosY");
        
        std::shared_ptr<LevelData> ld = _world->getLevelData();
        
        for (std::shared_ptr<LevelEntry> entry : ld->getLevelEntries()) {
            std::string wKey = entry->waveKey;
            std::shared_ptr<WaveData> wd = _world->getAssetManager()->get<WaveData>(wKey);
            bool found = false;
            for (std::shared_ptr<WaveEntry> waveEntry : wd->getWaveEntries()) {
                if (waveEntry->key == waveId) {
                    std::shared_ptr<WaveEntry> we = WaveEntry::alloc(xPos, yPos,
                                                                     waveEntry->getElement(),
                                                                     waveEntry->getTemplateKey(),
                                                                     waveEntry->getAIKey());
                    spawnWaveEntry(we, false, state);
                    found = true;
                    break;
                }
            }
            if (found){
                break;
            }
        }
    }
    
    std::shared_ptr<JsonValue> playerList = levelControlJson->get("players");
    
    // for each character in the suspension file find the character in the levelData
    for (int i = 0; i < playerList->size(); i++) {
        std::shared_ptr<JsonValue> resumePlayer = playerList->get(i);
        
        // info from the serialized active objects of players
        std::string waveId = resumePlayer->getString("waveID");
        float xPos = resumePlayer->getFloat("currentPosX");
        float yPos = resumePlayer->getFloat("currentPosY");
        
        std::shared_ptr<LevelData> ld = _world->getLevelData();
        
        // find the approproriate wave entry of the character
        for (std::shared_ptr<WaveEntry> player : ld->getPlayerChars() ) {
            if (waveId != player->key){
                continue;
            }
            std::shared_ptr<WaveEntry> we = WaveEntry::alloc(xPos, yPos,
                                                             player->getElement(),
                                                             player->getTemplateKey(),
                                                             player->getAIKey());
            spawnWaveEntry(we, true, state);
            break;
        }
    }
    
    // put entries in uid to waveEntry mapping back in
    std::shared_ptr<JsonValue> uidToWaveEntryMapJson = levelControlJson->get("uidToWaveEntryMap");
    for (int i =0;i < uidToWaveEntryMapJson->size();i++){
        std::shared_ptr<JsonValue> entry = uidToWaveEntryMapJson->get(i);
        int uid = std::stoi(entry->key());
        std::string waveEntryKey = uidToWaveEntryMapJson->getString(entry->key());
        // map the uid of the gameObject to the waveEntry key used to identify it in the json (from the parent class data)
        _uidToWaveEntryMap.insert(std::make_pair(uid,waveEntryKey));
    }
}



std::string LevelController::serialize(std::shared_ptr<GameState> state){
	return toJsonValue(state)->toString();
}

std::shared_ptr<JsonValue> LevelController::toJsonValue(std::shared_ptr<GameState> state){
    
    // TODO follow the doc serialize info within the level and also the gameObjectuid->WaveEntryID
    // it is given the gameState because the active gameObject and the player characters are
    // used by the levelController deserialize to get back into the game so it is within this json
    
	std::shared_ptr<JsonValue> lc = JsonValue::allocObject();
	lc->appendChild("levelDataKey", JsonValue::alloc(_level.getLevelKey()));
	lc->appendChild("currentWaveIndex", JsonValue::alloc((float)_level.getCurrentWave()));
	lc->appendChild("framesElapsed", JsonValue::alloc(_level.getFramesElapsed()));
	lc->appendChild("readyToSpawn", JsonValue::alloc(_level.isReadyToSpawn() ? 1. : 0.));
	lc->appendChild("playerSpawned", JsonValue::alloc(_level.hasPlayerSpawned() ? 1. : 0.));

	std::shared_ptr<JsonValue> map = JsonValue::allocObject();
	for (auto entry : _uidToWaveEntryMap) {
		map->appendChild(std::to_string(entry.first), JsonValue::alloc(entry.second));
	}

	std::shared_ptr<JsonValue> players = JsonValue::allocArray();
	for (auto pl : state->getPlayerCharacters()) {
        // skip the player zones
        if (pl->type != GameObject::ObjectType::CHARACTER){
            continue;
        }
		std::shared_ptr<JsonValue> activeMap = JsonValue::allocObject();
		activeMap->appendChild("uid", JsonValue::alloc(std::to_string(pl->getUid())));
		activeMap->appendChild("currentPosX", JsonValue::alloc(pl->getPosition().x));
		activeMap->appendChild("currentPosY", JsonValue::alloc(pl->getPosition().y));
		activeMap->appendChild("waveID", JsonValue::alloc(_uidToWaveEntryMap.at(pl->getUid())));

		players->appendChild(activeMap);
	}

	std::shared_ptr<JsonValue> enemies = JsonValue::allocArray();
	for (auto en : state->getEnemyObjects()) {
		std::shared_ptr<JsonValue> activeMap = JsonValue::allocObject();
		activeMap->appendChild("uid", JsonValue::alloc(std::to_string(en->getUid())));
		activeMap->appendChild("currentPosX", JsonValue::alloc(en->getPosition().x));
		activeMap->appendChild("currentPosY", JsonValue::alloc(en->getPosition().y));
		activeMap->appendChild("waveID", JsonValue::alloc(_uidToWaveEntryMap.at(en->getUid())));
		enemies->appendChild(activeMap);
	}

	lc->appendChild("players", players);
	lc->appendChild("enemyObjects", enemies);
	lc->appendChild("uidToWaveEntryMap", map);


	return lc;
}
