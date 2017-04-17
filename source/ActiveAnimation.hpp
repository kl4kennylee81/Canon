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
#include "AnimationAction.hpp"
#include "GameState.hpp"

class ActiveAnimation {
private:
    std::shared_ptr<cugl::AnimationNode> _node;
    
    std::shared_ptr<AnimationData> _data;
    
    std::string active;

    std::string repeat;
    
    int curIndex;
    
    float curFrames;
    
    /**
     * If this is true, then the active animation is ready to be removed.
     */
    bool _last;
    
    bool flash;
    
public:
    bool lit;
    float flashIndex;
    
    ActiveAnimation():_node(nullptr),_data(nullptr),_last(false),curIndex(0),curFrames(0){}
    
    ~ActiveAnimation(){ dispose(); }
    
    void dispose();
    
    bool init() {
        _last = false;
        _node = nullptr;
        _data = nullptr;
        active = std::string();
        repeat = std::string();
        curIndex = 0;
        curFrames = 0;
        flash = false;
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
    
    bool isUniformScaling();
    
    std::shared_ptr<AnimationState> getAnimationState() {
        std::string state = repeat;
        if (!active.empty()){
            state = active;
        }
        return _data->getAnimationState(state);
    }
    
    std::shared_ptr<AnimationUpdate> getAnimationUpdate(AnimationAction action) {
        return _data->getAnimationUpdate(action);
    }
    
    bool eventExists(AnimationAction action) {
        return _data->actionExists(action);
    }
    
    void handleAction(AnimationAction action);
    
    bool nextFrame();
    
    void setLastAnimation() {_last = true;};
    
    bool isLastAnimation() { return _last;};
    
    void setScale(float scale){
        _node->setScale(scale);
    }
    
    void setFlash(bool f) {
        flash = f;
        lit = false;
        flashIndex = 0;
    }
    
    bool getFlash() {return flash;};
};
#endif /* ActiveAnimation_hpp */
