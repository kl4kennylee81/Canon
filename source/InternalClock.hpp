//
//  InternalClock.hpp
//  Canon
//
//  Created by Hong Jeon on 3/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef InternalClock_hpp
#define InternalClock_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class InternalClock {
private:
    /**
     * Total number of frames elapsed since the beginning of the game
     */
    float _totalFrames;
    
    /**
     * the scaling factor for how many frames to advance per one update() call.
     * Normal value is 1
     */
    float _timeDilation;
    
    
public:
    InternalClock():_totalFrames(0),_timeDilation(1) {}
    
    float getTotalTime() {
        return _totalFrames;
    }
    
    float getTimeDilation() {
        return _timeDilation;
    }
    
    /**
     * Increments the global time by the given step
     */
    void incrementTime(float step) {
        _totalFrames += step;
    }
    
    /**
     * Sets the time dilation by the given scale amount
     */
    void setTimeDilation(float scale) {
        _timeDilation = scale;
    }
    
    
    bool init() {
        _totalFrames = 0;
        _timeDilation = 1;
        return true;
    }
    
    static std::shared_ptr<InternalClock> alloc() {
        std::shared_ptr<InternalClock> result = std::make_shared<InternalClock>();
        return (result->init() ? result : nullptr);
    }
};

#endif /* InternalClock_hpp */
