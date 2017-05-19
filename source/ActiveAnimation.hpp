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
#include "Element.hpp"
#include <math.h>

class ActiveAnimation {
private:
    
    // physicsShape * animationScale = actual animation texture size
    // this is to provide a buffer for the texture to be smaller than the shape
    float _animationScale;
    
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
    
    bool hitStun;
    
public:
    bool lit;
    float flashIndex;
    
    ActiveAnimation():_node(nullptr),_data(nullptr),_last(false),curIndex(0),curFrames(0),_animationScale(1.0){}
    
    ~ActiveAnimation(){ dispose(); }
    
    void dispose();
    
    bool init(float animScale) {
        _last = false;
        _node = nullptr;
        _data = nullptr;
        active = std::string();
        repeat = std::string();
        curIndex = 0;
        curFrames = 0;
        flash = false;
        hitStun = false;
        this->_animationScale = animScale;
        return true;
    }
    
    static std::shared_ptr<ActiveAnimation> alloc(float animScale=1.0f) {
        std::shared_ptr<ActiveAnimation> result = std::make_shared<ActiveAnimation>();
        return (result->init(animScale) ? result : nullptr);
    }
    
    void setAnimationData(std::shared_ptr<AnimationData> data) {
        _data = data;
        _node = cugl::AnimationNode::alloc(_data->texture, _data->rows, _data->cols, _data->size);
        float ang = data->angle/90*M_PI/2;
        _node->setAngle(ang);
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
    
    std::shared_ptr<cugl::AnimationNode> getNode() { return _node; }
    
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
    
    float getAnimationScale() { return _animationScale; };
    
    void setHitStun(bool h) {
        hitStun = h;
        lit = false;
        flashIndex = 0;
    }
    
    bool getHitStun() {return hitStun;} ;
    
    bool usePhysicsAngle() {return _data->physicsAngle;};
};
#endif /* ActiveAnimation_hpp */
