//
//  BulletInitEvent.hpp
//  Canon
//
//  Created by Kyle Sampson on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef BulletInitEvent_hpp
#define BulletInitEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"
#include "BulletData.hpp"
#include "Element.hpp"
#include "GameState.hpp"


class BulletInitEvent : public Event {
public:
    std::shared_ptr<BulletData> _bulletData;
    float angle;
    ElementType element;
    cugl::Vec2 position;
    std::shared_ptr<GameState> state;
    
    BulletInitEvent() : Event() {
        this->_eventType = EventType::BULLET;
    }
    
    bool init(std::shared_ptr<BulletData> dat, float ang, ElementType el, cugl::Vec2 pos, std::shared_ptr<GameState> st){
        _bulletData = dat;
        angle = ang;
        position = pos;
        element = el;
        state = st;
        return true;
    }
    
    static std::shared_ptr<BulletInitEvent> alloc(std::shared_ptr<BulletData> dat, float ang, ElementType el, cugl::Vec2 pos, std::shared_ptr<GameState> st){
        std::shared_ptr<BulletInitEvent> result = std::make_shared<BulletInitEvent>();
        return (result->init(dat, ang, el, pos, st) ? result : nullptr);
    }
};

#endif /* BulletInitEvent_hpp */
