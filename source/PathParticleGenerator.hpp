//
//  PathParticleGenerator.hpp
//  Canon
//
//  Created by Hong Jeon on 5/19/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathParticleGenerator_hpp
#define PathParticleGenerator_hpp

#include <stdio.h>
#include "ParticleGenerator.hpp"
#include "PathController.hpp"


class PathParticleGenerator : public ParticleGenerator {
private:
    std::shared_ptr<ParticleNode> _bluepartnode; // one particlenode per texture for now
    std::shared_ptr<ParticleNode> _goldpartnode;
    
    ParticleData _blue_path_pd;
    ParticleData _gold_path_pd;
    
    ElementType _active_element;
    
    /**
     * This shared pointer refers to the one path. Address doesn't change after initial assignment.
     */
    std::shared_ptr<Path> _path;
    
    /**
     * Size of the path the last time we saw it
     */
    size_t _last_size;
    
    /**
     * Endpoint the last time we saw the path.
     */
    Vec2 _last_point;
    
    void update_paths(int group_num);
    
    void add_particles(int group_num, std::vector<Vec2> locations);
    
public:
    PathParticleGenerator() : ParticleGenerator() {}
    
    bool init(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map);
    
    static std::shared_ptr<PathParticleGenerator> alloc(std::shared_ptr<GameState> state, std::unordered_map<std::string, ParticleData>* particle_map) {
        std::shared_ptr<PathParticleGenerator> result = std::make_shared<PathParticleGenerator>();
        return (result->init(state, particle_map) ? result : nullptr);
    }
    
    void generate(std::shared_ptr<GameState> state);
    
    /**
     * Gets called on path init. We keep a reference to this same path and check its size
     * to see if it started over with another path.
     */
    void add_path(std::shared_ptr<Path> path, ElementType element);
    
    void remove_path(GameObject* active_char);
    
    std::vector<cugl::Vec2> interpolate(cugl::Vec2 fst, cugl::Vec2 snd, float dist, float thresh){
        std::vector<cugl::Vec2> coords = {};
        cugl::Vec2 step = snd-fst;
        
        if (step.length() == 0) {
            return coords;
        }
        
        if (step.length() < thresh) {
            return coords;
        }
        
        float length = step.length();
        step.normalize();
        step.scale(dist);
        float totSteps = length/dist;
        for (int i = 0; i < totSteps; i++){
            cugl::Vec2 toAdd = fst+step*i;
            coords.push_back(toAdd);
        }
        return coords;
    }

};


#endif /* PathParticleGenerator_hpp */
