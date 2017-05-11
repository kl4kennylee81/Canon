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
    ON_ENEMY_CLEARED
};

class TutorialStep {
private:
    TutorialTransition _transition;
    std::shared_ptr<UIComponent> _uiComponent;
    
public:
    TutorialStep():
    _uiComponent(nullptr){};
    
    ~TutorialStep(){};
    
    bool init();
    
    static std::shared_ptr<TutorialStep> alloc() {
        std::shared_ptr<TutorialStep> result = std::make_shared<TutorialStep>();
        return (result->init() ? result : nullptr);
    };
};

#endif /* TutorialStep_hpp */
