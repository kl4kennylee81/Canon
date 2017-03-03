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

#define ENEMY_SHAPE         "enemy_shape"

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
    int waveKey = _level.pollWave();
    if (waveKey != -1){
        // spawn the gameObject from the prototypes
        std::shared_ptr<WaveData> wd = _world->getWaveData(waveKey);
        for(auto it: wd->getWaveEntries()) {
            std::shared_ptr<ObjectData> od = _world->getObjectData(it->objectKey);
            std::shared_ptr<ShapeData> sd = _world->getShapeData(od->shape_id);
            
            auto image = _world->getAssetManager()->get<Texture>(ENEMY_SHAPE);
            auto enemyNode = PolygonNode::allocWithTexture(image);
            enemyNode->setAnchor(Vec2::ANCHOR_MIDDLE);
            enemyNode->setPosition(it->position);
            
            if (od->getElement() == Element::BLUE){
                enemyNode->setColor(Color4::BLUE);
            } else {
                enemyNode->setColor(Color4::YELLOW);
            }
            
            std::shared_ptr<GameObject> enemy = GameObject::alloc(enemyNode);
            enemy->setIsPlayer(false);
            
            auto box1 = BoxObstacle::alloc(enemy->getNode()->getPosition(), enemy->getNode()->getSize());
            std::shared_ptr<PhysicsComponent> physics1 = PhysicsComponent::alloc(box1, od->getElement());
            enemy->setPhysicsComponent(physics1);
            
            state->addGameObject(enemy);
            
            Event event = ObjectSpawnEvent::ObjectSpawnEvent(enemy);
            
            // notify the observers of the object that is spawned
            notify(&event);
            
        }
    }
    _level.update(timestep);
}

bool LevelController::init(std::shared_ptr<World> world) {
    _world = world;
    _level.init(world->getLevelData());
    return true;
}
