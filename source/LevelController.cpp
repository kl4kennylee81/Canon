//
//  LevelController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelController.hpp"
#include "LevelEvent.hpp"
#include "BulletInitEvent.hpp"
#include "BulletSpawnEvent.hpp"
#include "BulletAIData.hpp"
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
void LevelController::eventUpdate(Event* e) {
    switch (e->_eventType) {
        case Event::EventType::BULLET:
        {
            BulletInitEvent* bulletEvent = (BulletInitEvent*)e;
            Vec2 pos = bulletEvent->position;
            std::shared_ptr<ObjectData> od = _world->getObjectData(bulletEvent->_bulletData->getObjectKey());
            std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
            std::shared_ptr<AnimationData> animationd = _world->getAnimationData(od->getAnimationKey(bulletEvent->element));
            std::shared_ptr<SoundData> sounddata = _world->getSoundData(od->getSoundKey());
            
            std::shared_ptr<GameObject> gameOb = GameObject::alloc();
            gameOb->setIsPlayer(false);
            gameOb->type = GameObject::ObjectType::BULLET;
            
            auto ai = BulletAIData::alloc(bulletEvent->_bulletData->getVelocity(), bulletEvent->angle);
            //TODO: Change this shit
            std::shared_ptr<WaveEntry> we = WaveEntry::alloc(pos.x *32, pos.y*32, bulletEvent->element, "", "");
            
            std::shared_ptr<ObjectInitEvent> initevent =
                ObjectInitEvent::alloc(gameOb, we, od, animationd, sd, sounddata, ai, {}, nullptr);
            notify(initevent.get());
            
            bulletEvent->state->addEnemyGameObject(gameOb);
//            std::shared_ptr<ObjectSpawnEvent> spawn = ObjectSpawnEvent::alloc(gameOb);
//            notify(spawn.get());
//            
            std::shared_ptr<ObjectSpawningEvent> spawningevent = ObjectSpawningEvent::alloc(gameOb,1);
            
//             notify the observers of the object that is spawned
            notify(spawningevent.get());
//            std::shared_ptr<BulletSpawnEvent> bulletSpawn =
//                BulletSpawnEvent::alloc(gameOb, bulletEvent->_bulletData->getVelocity(), bulletEvent->angle);
//            notify(bulletSpawn.get());
            
            break;
        }
    }
}

void LevelController::spawnWaveEntry(std::shared_ptr<WaveEntry> we, bool isPlayer,std::shared_ptr<GameState> state){
    std::shared_ptr<TemplateWaveEntry> templated = _world->getTemplate(we->getTemplateKey());
    std::shared_ptr<ObjectData> od = _world->getObjectData(we);
    std::shared_ptr<ShapeData> sd = _world->getShapeData(od->getShapeKey());
    std::shared_ptr<AnimationData> animationd = _world->getAnimationData(od->getAnimationKey(we->getElement()));
    std::shared_ptr<AIData> aid = _world->getAIData(we); // aiKey is in the wave entry
    std::shared_ptr<BulletData> bd = _world->getBulletData(we);
    std::shared_ptr<SoundData> sounddata = _world->getSoundData(od->getSoundKey());
    if (sounddata == nullptr){
        sounddata = _world->getSoundData("baseEnemySound");
    }
    std::vector<std::shared_ptr<ZoneData>> zds = {};
    for (std::string zoneKey:_world->getZoneKeys(we)){
        zds.push_back(_world->getZoneData(zoneKey));
    }


    
    std::shared_ptr<GameObject> gameOb = GameObject::alloc();
    gameOb->setIsPlayer(isPlayer);
    
    std::shared_ptr<ObjectInitEvent> initevent = ObjectInitEvent::alloc(gameOb, we, od, animationd, sd, sounddata, aid, zds, bd);
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
