//
//  ZoneParticleGenerator.hpp
//
//  Created by Hong Jeon on 5/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ZoneParticleGenerator_hpp
#define ZoneParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"

/**
 * Creates the wavy effect we use for zones. Square zones TBD..
 */
class ZoneParticleGenerator : public ParticleGenerator {
private:
    std::shared_ptr<ParticleNode> _circlepartnode;
    std::shared_ptr<ParticleNode> _ringpartnode;
    ParticleData _circlepd;
    ParticleData _ringpd;
    
    int _ring_num;
    
    void createZoneParticle(int num, std::set<Particle*>& particle_set, ElementType element);
    ParticleData randomizePD(ParticleData pd);
    
    /**
     * This will be replaced by GameObject -> ParticleWrapper
     */
//    std::unordered_map<GameObject*, std::shared_ptr<ParticleWrapper>> _obj_to_wrapper;
    
//    void updateWrapper(std::shared_ptr<ParticleWrapper> wrapper, std::set<Particle*>& reset);
    
public:
    ZoneParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map);
    
    static std::shared_ptr<ZoneParticleGenerator> alloc(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
        std::shared_ptr<ZoneParticleGenerator> result = std::make_shared<ZoneParticleGenerator>();
        return (result->init(state, particle_map) ? result : nullptr);
    }
    
    void generate();
    
    void add_mapping(GameObject* obj);
    
    void remove_mapping(GameObject* obj);
};

#endif /* ZoneParticleGenerator_hpp */
