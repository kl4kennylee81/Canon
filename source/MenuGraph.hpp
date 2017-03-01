//
//  MenuGraph.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef MenuGraph_hpp
#define MenuGraph_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Mode.hpp"

class MenuGraph {
protected:
    Mode _currentMode;
public:
    MenuGraph();
    
    void setMode(Mode mode);
    
    Mode getMode();
};

#endif /* MenuGraph_hpp */
