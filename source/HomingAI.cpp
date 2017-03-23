#include "HomingAI.hpp"
#include "MoveController.hpp"
#include "PathParameters.h"

using namespace cugl;

void HomingAI::update(std::shared_ptr<GameState> state) {
    Element e = _object->getPhysicsComponent()->getElementType();
    
    // HACK we should check the actual active player list
    // and then iterate through to find closest one
    // no assumption on only 2
    int playerIndex = -1;
    
    for (int i = 0;i<state->getNumberPlayerCharacters();i++){
        std::shared_ptr<GameObject> player = state->getPlayer(i);
        if (e == player->getPhysicsComponent()->getElementType()){
            continue;
        }
        playerIndex = i;
    }
    
    // skip if suitable target not found
    if (playerIndex == -1){
        return;
    }
    
    auto player = state->getPlayerCharacters().at(playerIndex);
    cugl::Vec2 playerPos = player->getPosition();
    cugl::Vec2 enemyPos = _object->getPosition();
    
    //We should get speed from ai data file
    cugl::Vec2 direction = MoveController::getVelocityVector(enemyPos, playerPos, AI_SPEED * 60);
    _object->getPhysicsComponent()->getBody()->setLinearVelocity(direction);
}

bool HomingAI::garbageCollect(GameObject* obj) {
    return true;
}

bool HomingAI::isActive(){
    return _isActive;
}

void HomingAI::toggleActive(){
    _isActive = !_isActive;
}
