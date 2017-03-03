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

class ActiveAnimation {
    
    std::shared_ptr<cugl::AnimationNode> _node;
    
public:
    
    bool init() {
        return true;
    }
    
    static std::shared_ptr<ActiveAnimation> alloc() {
        std::shared_ptr<ActiveAnimation> result = std::make_shared<ActiveAnimation>();
        return (result->init() ? result : nullptr);
    }
};
#endif /* ActiveAnimation_hpp */
