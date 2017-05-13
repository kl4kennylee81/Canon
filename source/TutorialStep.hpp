//
//  TutorialStep.hpp
//  Canon
//
//  Created by Kenneth Lee on 5/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TutorialStep_hpp
#define TutorialStep_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "UIComponent.hpp"

enum class TutorialTransition : int {
    ON_CLICK,
    ON_PATH_DRAWN,
    ON_ENEMY_CLEARED,
    NONE
};

class TutorialStep {
private:
    TutorialTransition _startCondition;
    TutorialTransition _endCondition;
    std::shared_ptr<UIComponent> _uiComponent;
    
public:
    TutorialStep():
    _uiComponent(nullptr){};
    
    ~TutorialStep(){};
    
    bool init();
    
    void setUIComponent(std::shared_ptr<UIComponent> ui) {
        _uiComponent = ui;
    }
    
    void setTransition(TutorialTransition start, TutorialTransition end){
        _startCondition = start;
        _endCondition = end;
    }
    
    static std::shared_ptr<TutorialStep> alloc() {
        std::shared_ptr<TutorialStep> result = std::make_shared<TutorialStep>();
        return (result->init() ? result : nullptr);
    };
    
    TutorialTransition getStartCondition(){
        return _startCondition;
    }
    
    TutorialTransition getEndCondition(){
        return _endCondition;
    }
    
    std::shared_ptr<UIComponent> getUIComponent(){
        return _uiComponent;
    }
};

#endif /* TutorialStep_hpp */
