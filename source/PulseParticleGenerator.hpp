//
//  PulseParticleGenerator.hpp
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PulseParticleGenerator_hpp
#define PulseParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"
#include "GroupContainer.hpp"

/**
 * Create a pulase in the given locations. Used to indicate spawning enemies.
 */
class PulseParticleGenerator : public ParticleGenerator {
private:
    /**
     * Frames to wait until the next pulse is fired
     */
    float _timeout;
    
    /**
     * Main node for the pulse. Replace later with a list.
     */
    std::shared_ptr<ParticleNode> _pulsepartnode;
    
    /**
     * This also might need to be replace with a general list.
     */
    ParticleData _ringpd;
    
    /**
     * Group allocator
     */
    std::shared_ptr<GroupContainer> _groups;
    
    /**
     * GameObject -> group number of the wrapper
     */
    // maybe try changing this to something else if it lags
    std::unordered_map<GameObject*, int> _obj_to_group_num;
    
    void createPulseParticles(Vec2 world_pos, int group_num, ElementType element);
    
    ParticleData randomizeAngle(ParticleData pd);
    
//    void updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset);
    
public:
    PulseParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map);
    
    static std::shared_ptr<PulseParticleGenerator> alloc(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
        std::shared_ptr<PulseParticleGenerator> result = std::make_shared<PulseParticleGenerator>();
        return (result->init(state, particle_map) ? result : nullptr);
    }
    
    void generate();
    
    /**
     * Adds a gameobject to a list to generate a pulse at that location.
     */
    void add_mapping(GameObject* obj);
    
    /**
     * Removes the gameobject. Use this when you want to get rid of the pulse on the object.
     */
    void remove_mapping(GameObject* obj);
};


#endif /* PulseParticleGenerator_hpp */
