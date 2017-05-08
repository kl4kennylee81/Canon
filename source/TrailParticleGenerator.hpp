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
    std::set<Particle*> _particles; // the particles that are to be remoevd
    std::vector<GameObject*> _character_list;
    
    std::shared_ptr<ParticleNode> _bluepartnode;
    std::shared_ptr<ParticleNode> _goldpartnode;
    
    ParticleData _bluepd;
    ParticleData _goldpd;
    
    /**
     * Given a character Gameobj, generates a trail of the character's color at the character's 
     * position. This method uses _cooldown to determine how fast to spawn the trail particles.
     */
    void generate_trail(GameObject* char_obj);
    
    /**
     * This creates the particles by num count and also inserts into the particle node
     */
    void createTrailParticle(int num, ParticleData pd, std::shared_ptr<ParticleNode> partnode, Vec2 world_pos);
    
public:
    TrailParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<cugl::FreeList<Particle>> mem, std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map);
    
    static std::shared_ptr<TrailParticleGenerator> alloc(std::shared_ptr<cugl::FreeList<Particle>> mem, std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
        std::shared_ptr<TrailParticleGenerator> result = std::make_shared<TrailParticleGenerator>();
        return (result->init(mem, state, particle_map) ? result : nullptr);
    }
    
    /**
     * Adds character to the main list and everyone in the list gets a trail of their color.
     */
    void add_character(GameObject* char_obj);
    
    void remove_character(GameObject* char_obj);
    
    void generate();
};


#endif /* TrailParticleGenerator_hpp */
