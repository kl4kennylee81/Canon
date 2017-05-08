//
//  ActiveBullet.cpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "ActiveBullet.hpp"
#include "Element.hpp"

using namespace cugl;


std::vector<std::shared_ptr<BulletInitEvent>> ActiveBullet::update(std::shared_ptr<GameState> state) {
    _frameCount = (_frameCount + 1) % _bulletData->getCooldown();
    if(_frameCount == 0){
        return shootBullets(state);
    }
    return {};
}

std::vector<std::shared_ptr<BulletInitEvent>> ActiveBullet::shootBullets(std::shared_ptr<GameState> state){
    float targetAngle = 0;
    ElementType element = _object->getPhysicsComponent()->getElementType();
    for(auto it: state->getPlayerCharacters()) {
        if(it->getPhysicsComponent()->getElementType() != element ){
            Vec2 targetVector = Vec2::Vec2(_object->getPosition()).subtract(it->getPosition());
            targetAngle = targetVector.getAngle();
        }
    }
    
    float var = _bulletData->getAngleVariance();
    
    float angle = _bulletData->isTarget() ?
        targetAngle - (CU_MATH_DEG_TO_RAD(var) / 2) :
        CU_MATH_DEG_TO_RAD(_bulletData->getStartingAngle());
    
    std::vector<std::shared_ptr<BulletInitEvent>> bullets;
    for(int i = 0; i < _bulletData->getNumberShots(); i++) {
        bullets.push_back(BulletInitEvent::alloc(_bulletData, angle, element, _object->getPosition(), state));
        angle = _bulletData->isTarget() ?
            angle + (CU_MATH_DEG_TO_RAD(var) / (_bulletData->getNumberShots()-1)) :
            angle + (CU_MATH_DEG_TO_RAD(360) / _bulletData->getNumberShots());
        
    }
    return bullets;
}
