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

#define ENEMY_SHAPE         "enemy_shape_rect"

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
    int waveKey = _level.pollWave();
    if (waveKey != -1){
        // spawn the gameObject from the prototypes
        std::shared_ptr<WaveData> wd = _world->getWaveData(waveKey);
        for(auto it: wd->getWaveEntries()) {
            std::shared_ptr<ObjectData> od = _world->getObjectData(it->objectKey);
            std::shared_ptr<ShapeData> sd = _world->getShapeData(od->shape_id);
            std::shared_ptr<AnimationData> ad = _world->getAnimationData(od->animation_id);
			std::shared_ptr<AIData> ai = _world->getAIData(it->aiKey);
            std::shared_ptr<GameObject> enemy = GameObject::alloc();
            enemy->setIsPlayer(false);
            
            std::shared_ptr<ObjectInitEvent> initevent = ObjectInitEvent::alloc(enemy, it, od, ad, sd, ai);
            notify(initevent.get());
            
            /*
             auto image = _world->getAssetManager()->get<Texture>(ENEMY_SHAPE);
             auto enemyNode = PolygonNode::allocWithTexture(image);
             enemyNode->setAnchor(Vec2::ANCHOR_MIDDLE);
             
             if (od->getElement() == Element::BLUE){
             enemyNode->setColor(Color4::BLUE);
             } else {
             enemyNode->setColor(Color4::YELLOW);
             }
             
             std::shared_ptr<GameObject> enemy = GameObject::alloc(enemyNode);
             if (!enemy){
             assert(false);
             }
             enemy->setIsPlayer(false);
             
             auto box1 = BoxObstacle::alloc(it->position, enemy->getNode()->getSize()/state->getPhysicsScale());
             
             std::shared_ptr<PhysicsComponent> physics1 = PhysicsComponent::alloc(box1, od->getElement());
             enemy->setPhysicsComponent(physics1);
             */
            
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
