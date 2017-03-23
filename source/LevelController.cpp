//
//  LevelController.cpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelController.hpp"
#include "LevelEvent.hpp"

using namespace cugl;

LevelController::LevelController():
BaseController(),
_world(nullptr){}

void LevelController::attach(std::shared_ptr<Observer> obs) {
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

void LevelController::update(float timestep,std::shared_ptr<GameState> state){
    
    // TODO Temporary code to reset level when finished until we have a discrete ending
    if (_level.isSpawningFinished()){
        _level.reset();
        _progressBarController->reset(state,_world);
        return;
    }
    
    _level.update(timestep);
    _progressBarController->update(state,_level);
    if (_level.isReadyToSpawn()){
        _level.toggleReadyToSpawn();
        std::string waveKey = _level.getCurrentWaveKey();
        // spawn the gameObject from the prototypes
        std::shared_ptr<WaveData> wd = _world->getWaveData(waveKey);
        for(auto it: wd->getWaveEntries()) {
            std::shared_ptr<ObjectData> od = _world->getObjectData(it->objectKey);
            std::shared_ptr<ShapeData> sd = _world->getShapeData(od->shapeKey);
            std::shared_ptr<AnimationData> animationd = _world->getAnimationData(od->animationKey);
            std::shared_ptr<AIData> aid = _world->getAIData(od->aiKey);
            
            std::shared_ptr<GameObject> enemy = GameObject::alloc();
            enemy->setIsPlayer(false);
            
            std::shared_ptr<ObjectInitEvent> initevent = ObjectInitEvent::alloc(enemy, it, od, animationd, sd, aid);
            notify(initevent.get());
            
            // enemy is added to the game state here
            state->addEnemyGameObject(enemy);
            
            std::shared_ptr<ObjectSpawningEvent> spawningevent = ObjectSpawningEvent::alloc(enemy);
            
            // notify the observers of the object that is spawned
            notify(spawningevent.get());
            
        }
    }
}

bool LevelController::init(std::shared_ptr<GameState> state, std::shared_ptr<World> world) {
    _world = world;
    _level.init(world->getLevelData());
    _progressBarController = ProgressBarController::alloc(state,world);
    return true;
}
