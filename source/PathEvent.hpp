//
//  PathEvent.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef PathEvent_hpp
#define PathEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Path.hpp"
#include "Event.hpp"

class PathEvent : Event {
    enum class PathEventType : int {
        /** Signal that a path is done */
        PATH_FINISHED
    };
};

class PathFinished : PathEvent {
    std::shared_ptr<Path> _path;
};

#endif /* PathEvent_hpp */
