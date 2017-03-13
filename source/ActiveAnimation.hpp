//
//  ActiveAnimation.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActiveAnimation_hpp
#define ActiveAnimation_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <map>
#include "AnimationData.hpp"

class ActiveAnimation {
    
    std::shared_ptr<cugl::AnimationNode> _node;
    
    std::shared_ptr<AnimationData> _data;
    
    std::string active;

    std::string repeat;
    
    int curIndex;
    
    int curFrames;
    
    bool _last;
    
    
public:
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<ActiveAnimation> alloc() {
        std::shared_ptr<ActiveAnimation> result = std::make_shared<ActiveAnimation>();
        return (result->init() ? result : nullptr);
    }
    
    void setAnimationData(std::shared_ptr<AnimationData> data) {
        _data = data;
        _node = cugl::AnimationNode::alloc(_data->texture, _data->rows, _data->cols, _data->size);
    }
    
    std::shared_ptr<cugl::AnimationNode> getAnimationNode() {
        return _node;
    }
    
    std::shared_ptr<AnimationState> getAnimationState() {
        std::string state = repeat;
        if (!active.empty()){
            state = active;
        }
        return _data->getAnimationState(state);
    }
    
    std::shared_ptr<AnimationUpdate> getAnimationUpdate(AnimationEvent event) {
        return _data->getAnimationUpdate(event);
    }
    
    bool eventExists(AnimationEvent event) {
        return _data->eventExists(event);
    }
    
    void handleEvent(AnimationEvent event);
    
    bool nextFrame();
    
    void setLastAnimation() {_last = true;};
};
#endif /* ActiveAnimation_hpp */
