//
//  GroupContainer.hpp
//  Canon
//
//  Created by Hong Jeon on 5/18/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GroupContainer_hpp
#define GroupContainer_hpp

#include <stdio.h>
#include <set>
#include "Particle.h"
#include <cassert>

/**
 * Sometimes there is a need to move groups of particles as one entity. This class
 * encapsulates the methods and states that allow for transformation and updates
 * to happen as a group.
 */
class Group {
public:
    
    /**
     * Every particle in the group has the same position
     */
    bool position;
    
    /**
     * The position for all particles encapulated by this wrapper. In update, all the particles
     * positions get updates to match the global position.
     */
    Vec2 global_position;
    
    /**
     * Trigger group fade
     */
    bool fade;
    int fade_time;
    
    /**
     * Resetting the ttl of the particle after it has died. Mainly for repeating effects
     * like pulsing particle.
     */
    bool repeat;
    
    /**
     * Mark this as true when you want to replace this in the group container.
     */
    bool alive;
    
    /**
     * Use this constructor to initialize the array
     */
    Group() {
        // all default values
        global_position = Vec2(0,0);
        repeat = true;
        alive = false;
        position = true;
        fade = false;
    }
    
    /**
     * Use this to actually set the group
     */
    bool init(Vec2 pos, bool r) {
        global_position = pos;
        repeat = r;
        alive = true; // default
        
        return true;
    }
};

class GroupContainer {
public:
    int _max_size;
    int _next_available;
    
    // expose the underlying array
    Group* group_array;
    
    bool init(int max_size) {
        // instance variables
        _max_size = max_size;
        _next_available = 0;
        
        // make the underlying array
        group_array = new Group[max_size];
        
        return true;
    }
    
    static std::shared_ptr<GroupContainer> alloc(int max_size) {
        std::shared_ptr<GroupContainer> result = std::make_shared<GroupContainer>();
        return (result->init(max_size) ? result : nullptr);
    }
    
    /**
     * Makes a new group and returns that group number.
     * Reinitialize everything in the new group.
     */
    int makeNewGroup(Vec2 world_pos, bool repeat) {
        int group_num = -1;
        
        // go through the circular buffer and find the first not active one
        for (int i = 0; i < _max_size; i++) {
            int index = (_next_available+i) % _max_size;
            // check if active
            if (!group_array[index].alive) {
                // initialze this group. automatically alive = true
                group_array[index].init(world_pos, repeat);
                group_num = index;
                // update the next avail as the one right after this one
                _next_available = (index+1) % _max_size;
                break;
            }
        }
        
        // should always find one, if not, we need to increase _max_size
        assert(group_num >= 0);
        
        return group_num;
    }
};



#endif /* GroupContainer_hpp */
