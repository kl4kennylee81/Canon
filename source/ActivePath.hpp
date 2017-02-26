//
//  ActivePath.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef ActivePath_hpp
#define ActivePath_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Path.hpp"

class ActivePath {

    std::shared_ptr<Path> _path;
    int pathIndex;
};

#endif /* ActivePath_hpp */
