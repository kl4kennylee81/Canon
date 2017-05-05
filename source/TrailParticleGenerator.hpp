//
//  TrailParticleGenerator.hpp
//  Canon
//
//  Created by Hong Jeon on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TrailParticleGenerator_hpp
#define TrailParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"
#include "Util.hpp"

/**
 * Particle generator that makes a trail of particles behind the given character
 * TODO Commented out the code regarding GameObject because we are testing currently in a demo
 */
class TrailParticleGenerator : public ParticleGenerator {
private:
    // need to keep track of the character to get positions
    std::shared_ptr<GameObject> _char;
    int _cooldown;
    int _respawn;
    std::set<Particle*> _particles;
    
public:
    TrailParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> ch, std::shared_ptr<GameState> state) {
        _memory = mem;
        _pd = pd;
        _char = ch;
        _respawn = 0;
        _cooldown = _respawn;
        
        // initialize particle node and attach to the world node
        _partnode = ParticleNode::allocWithTexture(_pd.texture);
        _partnode->setBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        _partnode->setPosition(Vec2::ZERO);
        _partnode->setContentSize(state->getWorldNode()->getContentSize());
        state->getWorldNode()->addChild(_partnode);
        
        return true;
    }
    
    static std::shared_ptr<TrailParticleGenerator> alloc(std::shared_ptr<cugl::FreeList<Particle>> mem, ParticleData pd, std::shared_ptr<GameObject> gameObj, std::shared_ptr<GameState> state) {
        std::shared_ptr<TrailParticleGenerator> result = std::make_shared<TrailParticleGenerator>();
        return (result->init(mem, pd, gameObj, state) ? result : nullptr);
    }
    
    void generate();
};


#endif /* TrailParticleGenerator_hpp */
