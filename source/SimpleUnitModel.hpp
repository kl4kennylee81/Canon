//
//  SimpleUnitModel.hpp
//  Canon
//
//  Created by Kenneth Lee on 2/25/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef SimpleUnitModel_hpp
#define SimpleUnitModel_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Element.hpp"

class SimpleUnitModel : public cugl::PolygonObstacle {
    
protected:
    std::shared_ptr<cugl::Node> sgNode;
    Element elementType;
};

#endif /* SimpleUnitModel_hpp */
